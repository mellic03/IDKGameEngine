#include "sys-transform.hpp"

#include <libidk/idk_log.hpp>
#include <libidk/idk_geometry.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>


static idk::EngineAPI *api_ptr;



void
idk::TransformSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;
}



void
idk::TransformSys::update( idk::EngineAPI &api )
{
    float dtime = api.getEngine().deltaTime();

    for (auto &cmp: idk::ECS2::getComponentArray<idk::TransformCmp>())
    {
        if (cmp.transform.position != cmp.transform.position)
        {
            cmp.transform = idk::Transform::fromGLM(glm::mat4(1.0f));
        }

        cmp.pitch = fmod(cmp.pitch, 2.0*M_PI);
        cmp.roll  = fmod(cmp.roll, 2.0*M_PI);
        cmp.yaw   = fmod(cmp.yaw, 2.0*M_PI);

        // if (cmp.is_dirty && ECS2::hasParent(cmp.obj_id) == false)
        if (ECS2::hasParent(cmp.obj_id) == false)
        {
            _computeTransform(cmp.obj_id, glm::mat4(1.0f));
        }

        // if (cmp.is_dirty)
        {
            // recomputeTransformMatrices(cmp.obj_id);
            // cmp.is_dirty = false;
        }
    }



    for (auto &cmp: ECS2::getComponentArray<IKCmp>())
    {
        if (cmp.obj_id == -1 || cmp.pole_target == -1)
        {
            continue;
        }

        glm::vec3 pole_target = getPositionWorldspace(cmp.pole_target);
        TransformSys::FABRIK(cmp.chain_length, cmp.obj_id, cmp.distances, pole_target);
    }


    for (auto &cmp: ECS2::getComponentArray<LookTowardCmp>())
    {
        if (cmp.obj_id == -1 || cmp.target_id == -1)
        {
            continue;
        }

        lookTowards(cmp.obj_id, cmp.target_id, 128.0f*dtime);

        // glm::vec3 target = getPositionWorldspace(cmp.pole_target);
        // TransformSys::FABRIK(cmp.chain_length, cmp.obj_id, cmp.distances, pole_target);
    }


    for (auto &[obj_id, anchor_ids, distances]: idk::ECS2::getComponentArray<idk::AnchorCmp>())
    {
        if (anchor_ids.back() != -1)
        {
            anchor_ids.push_back(-1);
            distances.push_back(1.0f);
        }

        glm::vec3 dir = glm::vec3(0.0f);

        for (int i=0; i<anchor_ids.size()-1; i++)
        {
            glm::vec3 anchor_pos = getPositionWorldspace(anchor_ids[i]);
            glm::vec3 obj_pos    = getPositionWorldspace(obj_id);

            float dist = glm::distance(anchor_pos, obj_pos);
            float error = distances[i] - dist;    

            dir += error*glm::normalize(obj_pos - anchor_pos);
        }

        dir /= anchor_ids.size();

        translateWorldspace(obj_id, dir);
    }


    for (auto &[obj_id, anchor_id, speed]: idk::ECS2::getComponentArray<idk::SmoothFollowCmp>())
    {
        glm::vec3 anchor_pos = getPositionWorldspace(anchor_id);
        glm::vec3 obj_pos    = getPositionWorldspace(obj_id);

        glm::vec3 dir = anchor_pos - obj_pos;
    
        translateWorldspace(obj_id, dtime*speed*dir);
    }


    for (auto &[obj_id, magnitude, axis]: ECS2::getComponentArray<RotateCmp>())
    {
        axis = glm::normalize(axis);
        glm::quat delta = glm::angleAxis(magnitude, axis);

        glm::quat &R = getLocalRotation(obj_id);
                   R = glm::normalize(delta * R);
    }

    _updateCallbacks();

}

void
idk::TransformSys::_computeTransform( int obj_id, const glm::mat4 &parent )
{
    auto &cmp = getTransformCmp(obj_id);

    idk::Transform T = cmp.transform;

    glm::mat4 current = parent * Transform::toGLM_noscale(T, cmp.pitch, cmp.roll, cmp.yaw);

    cmp.prev_world = cmp.world;
    cmp.prev_local = cmp.local;
    cmp.prev_model = cmp.model;

    cmp.world = parent;
    cmp.local = current;
    cmp.model = parent * Transform::toGLM(T, cmp.pitch, cmp.roll, cmp.yaw);

    cmp.up    = glm::normalize(glm::mat3(cmp.model) * glm::vec3(0.0f, 1.0f, 0.0f));
    cmp.right = glm::normalize(glm::mat3(cmp.model) * glm::vec3(1.0f, 0.0f, 0.0f));
    cmp.front = glm::normalize(glm::mat3(cmp.model) * glm::vec3(0.0f, 0.0f, -1.0f));

    for (int child_id: ECS2::getChildren(obj_id))
    {
        _computeTransform(child_id, current);
    }

    cmp.is_dirty = false;
}


void
idk::TransformSys::recomputeTransformMatrices( int obj_id )
{
    auto &cmp = getTransformCmp(obj_id);
    cmp.local = _computeLocalMatrix(obj_id, false);
    cmp.world = _computeWorldMatrix(obj_id);
    cmp.model = _computeModelMatrix(obj_id);

    cmp.up    = glm::normalize(glm::mat3(cmp.model) * glm::vec3(0.0f, 1.0f, 0.0f));
    cmp.right = glm::normalize(glm::mat3(cmp.model) * glm::vec3(1.0f, 0.0f, 0.0f));
    cmp.front = glm::normalize(glm::mat3(cmp.model) * glm::vec3(0.0f, 0.0f, -1.0f));

    cmp.world_rotation = glm::quat_cast(glm::mat4(glm::mat3(cmp.world * cmp.local)));
}



glm::mat4
idk::TransformSys::_computeLocalMatrix( int obj_id, bool scale )
{
    static const glm::mat4 ident = glm::mat4(1.0f);

    if (obj_id == -1)
    {
        return ident;
    }

    auto &cmp = getTransformCmp(obj_id);

    if (scale == true)
    {
        return idk::Transform::toGLM(cmp.transform, cmp.pitch, cmp.roll, cmp.yaw);
    }

    else
    {
        return idk::Transform::toGLM_noscale(cmp.transform, cmp.pitch, cmp.roll, cmp.yaw);
    }


    // cmp.up    = glm::normalize(cmp.up);
    // cmp.right = glm::normalize(glm::cross(cmp.front, cmp.up));
    // cmp.front = glm::normalize(glm::cross(cmp.up, cmp.right));

    // glm::quat Qroll  = glm::angleAxis(cmp.roll,  glm::vec3(0.0f, 0.0f, -1.0f));
    // glm::quat Qpitch = glm::angleAxis(cmp.pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    // glm::quat Qyaw   = glm::angleAxis(cmp.yaw,   glm::vec3(0.0f, 1.0f, 0.0f));

    // glm::mat4 Rroll  = glm::mat4_cast(Qroll);
    // glm::mat4 Rpitch = glm::mat4_cast(Qpitch);
    // glm::mat4 Ryaw   = glm::mat4_cast(Qyaw);

    // glm::mat4 R = glm::mat4_cast(cmp.rotation) * Ryaw * Rroll * Rpitch;
    // glm::mat4 T = glm::translate(ident, cmp.position);
    // glm::mat4 S = glm::scale(ident, (scale) ? cmp.scale * cmp.scale3 : glm::vec3(1.0f));

    // return T * R * S;
}


glm::mat4
idk::TransformSys::_computeWorldMatrix( int obj_id )
{
    static const glm::mat4 ident = glm::mat4(1.0f);
    
    if (obj_id == -1)
    {
        return ident;
    }

    int parent_id = idk::ECS2::getParent(obj_id);

    return _computeWorldMatrix(parent_id) * _computeLocalMatrix(parent_id, false);
}



glm::mat4
idk::TransformSys::_computeModelMatrix( int obj_id)
{
    return _computeWorldMatrix(obj_id) * _computeLocalMatrix(obj_id, true);
}





void
idk::TransformSys::translateWorldspace( int obj_id, const glm::vec3 &v )
{
    glm::mat4 M = glm::inverse(getWorldMatrix(obj_id));
    getLocalPosition(obj_id) += glm::mat3(M) * v;
}


void
idk::TransformSys::translateLocalspace( int obj_id, const glm::vec3 &v )
{
    glm::mat3 R = glm::mat3(getModelMatrix(obj_id));

    getLocalPosition(obj_id) += R * v;
}


void
idk::TransformSys::rotateLocalAxis( int obj_id, const glm::vec3 &axis, float theta )
{
    auto &T = getTransform(obj_id);
    auto  R = glm::angleAxis(theta, axis);

    T.rotation = glm::normalize(R * T.rotation);
}


void
idk::TransformSys::rotateWorldAxis( int obj_id, const glm::vec3 &axis, float theta )
{
    glm::mat3 R = glm::inverse(glm::mat3(getModelMatrix(obj_id)));
    rotateLocalAxis(obj_id, R*axis, theta);
}




idk::TransformCmp&
idk::TransformSys::getTransformCmp( int obj_id )
{
    auto &cmp = idk::ECS2::getComponent<TransformCmp>(obj_id);
          cmp.is_dirty = true;

    return cmp;
}


idk::Transform&
idk::TransformSys::getTransform( int obj_id )
{
    return getTransformCmp(obj_id).transform;
}


glm::vec3&
idk::TransformSys::getLocalPosition( int obj_id )
{
    return getTransform(obj_id).position;
}


glm::vec3
idk::TransformSys::getLocalPositionDelta( int obj_id )
{
    auto &cmp = getTransformCmp(obj_id);
    return cmp.local[3] - cmp.prev_local[3];
}


glm::quat&
idk::TransformSys::getLocalRotation( int obj_id )
{
    return getTransform(obj_id).rotation;

}


glm::vec4&
idk::TransformSys::getScale( int obj_id )
{
    return getTransform(obj_id).scale;
}


glm::vec3&
idk::TransformSys::getXYZScale( int obj_id )
{
    glm::vec4 *scale4 = &getScale(obj_id);
    return *reinterpret_cast<glm::vec3 *>(scale4);
}


float&
idk::TransformSys::getUniformScale( int obj_id )
{
    return getScale(obj_id).w;
}



glm::vec3
idk::TransformSys::getWorldPosition( int obj_id )
{
    return getTransformCmp(obj_id).model[3];
}


glm::vec3
idk::TransformSys::getWorldPositionDelta( int obj_id )
{
    auto &cmp = getTransformCmp(obj_id);
    return cmp.model[3] - cmp.prev_model[3];
}


glm::quat
idk::TransformSys::getWorldRotation( int obj_id )
{
    return getTransformCmp(obj_id).world_rotation;
}












glm::vec3
idk::TransformSys::getPositionWorldspace( int obj_id )
{
    return ECS2::getComponent<TransformCmp>(obj_id).model[3];
    // return glm::vec3(getModelMatrix(obj_id)[3]);
}


void
idk::TransformSys::setWorldPosition( int obj_id, const glm::vec3 &v )
{
    glm::vec3 delta = v - getWorldPosition(obj_id);
    translateWorldspace(obj_id, delta);
}


void
idk::TransformSys::setPositionLocalspace( int obj_id, const glm::vec3 &v )
{
    getLocalPosition(obj_id) = v;
}



glm::vec3
idk::TransformSys::getUp( int obj_id )
{
    return getTransformCmp(obj_id).up;
}


glm::vec3
idk::TransformSys::getRight( int obj_id )
{
    return getTransformCmp(obj_id).right;
}


glm::vec3
idk::TransformSys::getFront( int obj_id )
{
    return getTransformCmp(obj_id).front;
}


void
idk::TransformSys::setFront( int obj_id, const glm::vec3 &front )
{
    glm::vec3 dir = glm::inverse(glm::mat3(getModelMatrix(obj_id))) * front;

    glm::mat4 R = glm::lookAt(glm::vec3(0.0f), dir, glm::vec3(0.0f, 1.0f, 0.0f));
              R = glm::mat4(glm::mat3(R));
              R = glm::inverse(R);

    getTransform(obj_id).rotation = glm::normalize(glm::quat_cast(R));
}



glm::mat4
idk::TransformSys::getLocalMatrix( int obj_id, bool scale )
{
    if (obj_id == -1)
    {
        return glm::mat4(1.0f);
    }

    return getTransformCmp(obj_id).local;
}


glm::mat4
idk::TransformSys::getWorldMatrix( int obj_id )
{
    if (obj_id == -1)
    {
        return glm::mat4(1.0f);
    }

    return getTransformCmp(obj_id).world;
}



glm::mat4
idk::TransformSys::getModelMatrix( int obj_id)
{
    if (obj_id == -1)
    {
        return glm::mat4(1.0f);
    }

    return getTransformCmp(obj_id).model;
}


float
idk::TransformSys::getDistanceWorldspace( int a, int b )
{
    return glm::distance(getPositionWorldspace(a), getPositionWorldspace(b));
}


bool
idk::TransformSys::isInsideBoundingRect( int obj_id, const glm::vec3 &pos )
{
    const glm::mat4 M = glm::inverse(getModelMatrix(obj_id));
    const glm::vec3 local = glm::vec3(M * glm::vec4(pos, 1.0f));

    return fabs(local.x) < 0.5f && fabs(local.y) < 0.5f && fabs(local.z) < 0.5f;
}



glm::mat4 extractRotation1( const glm::mat4 &M )
{
    glm::vec3 r_scale;
    glm::quat r_rot;
    glm::vec3 r_trans;
    glm::vec3 r_skew;
    glm::vec4 r_persp;
    glm::decompose(M, r_scale, r_rot, r_trans, r_skew, r_persp);
    return glm::mat4_cast(r_rot);
}


float
idk::TransformSys::lookTowards( int subject, int target, float alpha )
{
    glm::vec3 A = glm::vec3(0.0f);
    glm::vec3 B = getPositionWorldspace(subject) - getPositionWorldspace(target);
    glm::mat4 V = glm::inverse(glm::lookAt(A, B, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::mat4 M = TransformSys::getWorldMatrix(subject);
    glm::mat4 R = glm::inverse(extractRotation1(M)) * V;

    TransformSys::getLocalRotation(subject) = glm::normalize(glm::quat_cast(R));
    // TransformSys::lookTowards(obj_id, elbow_pos, 0.5f);


    return 0.0f; // lookTowards(subject, getPositionWorldspace(target), alpha);
}


float
idk::TransformSys::lookTowards( int subject, const glm::vec3 &pos, float alpha )
{
    glm::vec3 dir = pos - getPositionWorldspace(subject);
              dir = glm::normalize(dir);

    glm::vec3 front = getFront(subject);
    glm::vec3 up    = getUp(subject);

    float dot   = glm::dot(dir, front);
    float det   = glm::dot(up, glm::cross(dir, front));
    float delta = atan2(det, dot);

    while (delta < -2.0*M_PI)
    {
        delta += 2.0*M_PI;
    }

    while (delta > +2.0*M_PI)
    {
        delta -= 2.0*M_PI;
    }

    // if (fabs(delta) > 3.14f)
    // {
    //     delta -= glm::sign(delta) * M_PI;
    // }

    TransformSys::yaw(subject, alpha*delta);

    return getTransformCmp(subject).yaw - delta;
}




float
idk::TransformSys::moveTowards( int subject, int target, float alpha )
{
    return moveTowards(subject, getPositionWorldspace(target), alpha);
}



float
idk::TransformSys::moveTowards( int subject, const glm::vec3 &pos, float alpha )
{
    glm::vec3 pos_A = pos;
    glm::vec3 pos_B = getPositionWorldspace(subject);

    glm::vec3 dir = (pos_A - pos_B);

    if (glm::length(dir) > 1.0f)
    {
        dir = glm::normalize(dir);
    }

    translateWorldspace(subject, alpha * dir);

    return 1.0f;
}



void
idk::TransformSys::lookTowardsCallback( int subject, int target, float alpha, float stop,
                                        std::function<void()> callback )
{
    TransformCallback TC = { subject, target, alpha, stop, callback };
    m_look_callbacks[subject] = TC;
}


void
idk::TransformSys::moveTowardsCallback( int subject, int target, float alpha, float stop,
                                        std::function<void()> callback )
{
    TransformCallback TC = { subject, target, alpha, stop, callback };
    m_move_callbacks[subject] = TC;
}


void
idk::TransformSys::_updateCallbacks()
{
    static std::unordered_set<int> finished_callbacks;
    finished_callbacks.clear();


    for (auto &[obj_id, TC]: m_look_callbacks)
    {
        float delta = lookTowards(TC.subject, TC.target, TC.alpha);

        if (delta <= TC.stop)
        {
            TC.callback();
            finished_callbacks.insert(obj_id);
        }
    }

    for (int obj_id: finished_callbacks)
    {
        m_look_callbacks.erase(obj_id);
    }



    for (auto &[obj_id, TC]: m_move_callbacks)
    {
        float dist = moveTowards(TC.subject, TC.target, TC.alpha);

        if (dist <= TC.stop)
        {
            TC.callback();
            finished_callbacks.insert(obj_id);
        } 
    }

    for (int obj_id: finished_callbacks)
    {
        m_move_callbacks.erase(obj_id);
    }

}


void
idk::TransformSys::FABRIK( const glm::vec3 &posA, glm::vec3 &posB, glm::vec3 &posC,
                           const glm::vec3 &pole_target,
                           float distAB, float distBC )
{
    glm::vec3 plane_dir = glm::normalize(glm::cross(posA-pole_target, posC-pole_target));
    float dist = idk::geometry::distPlanePoint(pole_target, plane_dir, posB);

    // Place middle joint on plane
    posB -= dist*plane_dir;

    glm::vec3 midpoint = 0.5f * (posA + posB);
    glm::vec3 pole_dir = glm::normalize(pole_target - midpoint);

    // Move middle joint towards pole target
    posB += 0.1f*pole_dir;


    glm::vec3 dir;

    for (int i=0; i<3; i++)
    {
        // Backward pass
        // -----------------------------------------------------
        dir  = glm::normalize(posB - posC);
        posB = posC + distBC*dir;

        // dir  = glm::normalize(posA - posB);
        // posA = posB + distAB*dir;
        // -----------------------------------------------------


        // Forward pass
        // -----------------------------------------------------
        // posA = start_pos;

        dir  = glm::normalize(posB - posA);
        posB = posA + distAB*dir;

        dir  = glm::normalize(posC - posB);
        posC = posB + distBC*dir;
        // -----------------------------------------------------
    }
}


void
idk::TransformSys::FABRIK( std::vector<glm::vec3> &positions,
                           const std::vector<float> &distances,
                           const glm::vec3 &pole_target )
{
    glm::vec3 root_pos = positions[0];
    glm::vec3 end_pos  = positions.back();


    float total_length = 0.0f;
    for (float d: distances)
    {
        total_length += d;
    }

    if (glm::distance(root_pos, end_pos) > total_length)
    {
        positions.back() = root_pos + total_length*glm::normalize(root_pos - end_pos);
        end_pos = positions.back();
    }


    glm::vec3 plane_normal = glm::cross(root_pos-pole_target, end_pos-pole_target);
              plane_normal = glm::normalize(plane_normal);


    glm::vec3 dir;

    for (int j=0; j<3; j++)
    {
        // Backward pass
        // -----------------------------------------------------
        for (int i=positions.size()-2; i>=0; i--)
        {
            glm::vec3 &left  = positions[i];
            glm::vec3 &right = positions[i+1];
            glm::vec3 mid    = 0.5f * (left + right);

            float plane_dist = idk::geometry::distPlanePoint(pole_target, plane_normal, right);
            right -= plane_dist*plane_normal;
        
            dir  = glm::normalize(left - right);
            left = right + distances[i]*dir;
        }
        // -----------------------------------------------------

        positions[0] = root_pos;

        // Forward pass
        // -----------------------------------------------------
        for (int i=0; i<positions.size()-1; i++)
        {
            glm::vec3 &left  = positions[i];
            glm::vec3 &right = positions[i+1];

            dir  = glm::normalize(right - left);
            right = left + distances[i]*dir;
        }
        // -----------------------------------------------------
    }
}


void
idk::TransformSys::FABRIK( std::vector<glm::vec3> &positions,
                           const std::vector<float> &distances )
{
    glm::vec3 root_pos = positions[0];
    glm::vec3 end_pos  = positions.back();


    float total_length = 0.0f;
    for (float d: distances)
    {
        total_length += d;
    }

    // if (glm::distance(root_pos, end_pos) > total_length)
    // {
    //     positions.back() = root_pos + total_length*glm::normalize(root_pos - end_pos);
    //     end_pos = positions.back();
    // }

    glm::vec3 dir;

    for (int j=0; j<3; j++)
    {
        // Backward pass
        // -----------------------------------------------------
        for (int i=positions.size()-2; i>=0; i--)
        {
            glm::vec3 &left  = positions[i];
            glm::vec3 &right = positions[i+1];
            glm::vec3 mid    = 0.5f * (left + right);
        
            dir  = glm::normalize(left - right);
            left = right + distances[i]*dir;
        }
        // -----------------------------------------------------

        positions[0] = root_pos;

        // Forward pass
        // -----------------------------------------------------
        for (int i=0; i<positions.size()-1; i++)
        {
            glm::vec3 &left  = positions[i];
            glm::vec3 &right = positions[i+1];

            dir  = glm::normalize(right - left);
            right = left + distances[i]*dir;
        }
        // -----------------------------------------------------
    }
}




void
idk::TransformSys::FABRIK( int chain_length, int end_obj, const std::vector<float> &distances,
                           const glm::vec3 &pole_target )
{
    std::vector<glm::vec3> positions(chain_length);

    int current = end_obj;

    for (int i=0; i<chain_length; i++)
    {
        positions[chain_length-i-1] = getPositionWorldspace(current);
        current = ECS2::getParent(current);
    }

    FABRIK(positions, distances, pole_target);


    current = end_obj;

    for (int i=0; i<chain_length; i++)
    {
        glm::vec3 dir = positions[chain_length-i-1] - getPositionWorldspace(current);
        translateWorldspace(current, 0.25f*dir);
        current = ECS2::getParent(current);
    }

    for (int i=0; i<chain_length-1; i++)
    {
        api_ptr->getRenderer().drawCapsule(
            positions[i], positions[i+1], 0.025f
        );
    }

}





void
idk::TransformSys::FABRIK( int objA, int objB, int objC, glm::vec3 end_pos,
                           float distAB, float distBC, const glm::vec3 &pole_target )
{
    

    glm::vec3 start_pos = getPositionWorldspace(objA);

    glm::vec3 posA = start_pos;
    glm::vec3 posB = getPositionWorldspace(objB);
    glm::vec3 posC = end_pos;
    glm::vec3 dir;



    glm::vec3 plane_dir = glm::normalize(glm::cross(posA-pole_target, posC-pole_target));
    float dist = idk::geometry::distPlanePoint(pole_target, plane_dir, posB);

    // Place middle joint on plane
    posB -= dist*plane_dir;

    // glm::vec3 midpoint = 0.5f * (posA + posB);
    // glm::vec3 pole_dir = glm::normalize(pole_target - midpoint);

    // // Move middle joint towards pole target
    // posB += 0.1f*pole_dir;


    for (int i=0; i<1; i++)
    {
        // Backward pass
        // -----------------------------------------------------
        posC = end_pos;

        dir  = glm::normalize(posB - posC);
        posB = posC + distBC*dir;

        dir  = glm::normalize(posA - posB);
        posA = posB + distAB*dir;
        // -----------------------------------------------------


        // Forward pass
        // -----------------------------------------------------
        posA = start_pos;

        dir  = glm::normalize(posB - posA);
        posB = posA + distAB*dir;

        dir  = glm::normalize(posC - posB);
        posC = posB + distBC*dir;
        // -----------------------------------------------------
    }

    setWorldPosition(objC, posC);
    setWorldPosition(objB, posB);
    setWorldPosition(objA, posA);
}


void
idk::TransformSys::pitch( int obj_id, float f )
{
    float &pitch = getTransformCmp(obj_id).pitch;
           pitch = glm::clamp(pitch + glm::radians(f), -1.45f, +1.45f);
}


void
idk::TransformSys::yaw( int obj_id, float f )
{
    getTransformCmp(obj_id).yaw += glm::radians(f);
}


void
idk::TransformSys::roll( int obj_id, float f )
{
    getTransformCmp(obj_id).roll += glm::radians(f);
}




// Convenience functions
// ---------------------------------------------------------------------------------------------
void
idk::TransformSys::moveUp( int obj_id, float f )
{
    translateWorldspace(obj_id, f*getUp(obj_id));
}


void
idk::TransformSys::moveRight( int obj_id, float f )
{
    translateWorldspace(obj_id, f*getRight(obj_id));
}


void
idk::TransformSys::moveFront( int obj_id, float f )
{
    translateWorldspace(obj_id, f*getFront(obj_id));
}
// ---------------------------------------------------------------------------------------------

