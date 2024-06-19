#include "idk_systems.hpp"
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
    auto &audio = api.getAudioSys();
    float dtime = api.getEngine().deltaTime();

    for (auto &cmp: idk::ECS2::getComponentArray<idk::TransformCmp>())
    {
        if (cmp.position != cmp.position)
        {
            cmp.position = glm::vec3(0.0f);
            cmp.position.x += (rand() % 100) / 100.0f;
            cmp.position.y += (rand() % 100) / 100.0f;
            cmp.position.z += (rand() % 100) / 100.0f;
        }

        glm::vec3 last_pos = glm::vec3(getModelMatrix(cmp.obj_id)[3]);

        recomputeTransformMatrices(cmp.obj_id);

        glm::vec3 curr_pos = glm::vec3(getModelMatrix(cmp.obj_id)[3]);

        cmp.delta = curr_pos - last_pos;
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

    _updateCallbacks();

}



void
idk::TransformSys::recomputeTransformMatrices( int obj_id )
{
    getData(obj_id).local = _computeLocalMatrix(obj_id, false);
    getData(obj_id).world = _computeWorldMatrix(obj_id);
    getData(obj_id).model = _computeModelMatrix(obj_id);
}



glm::mat4
idk::TransformSys::_computeLocalMatrix( int obj_id, bool scale )
{
    static const glm::mat4 ident = glm::mat4(1.0f);

    if (obj_id == -1)
    {
        return ident;
    }

    auto &cmp = getData(obj_id);

    cmp.up    = glm::normalize(cmp.up);
    cmp.right = glm::normalize(glm::cross(cmp.front, cmp.up));
    cmp.front = glm::normalize(glm::cross(cmp.up, cmp.right));

    glm::quat Qroll  = glm::angleAxis(cmp.roll,  glm::vec3(0.0f, 0.0f, -1.0f));
    glm::quat Qpitch = glm::angleAxis(cmp.pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat Qyaw   = glm::angleAxis(cmp.yaw,   glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 Rroll  = glm::mat4_cast(Qroll);
    glm::mat4 Rpitch = glm::mat4_cast(Qpitch);
    glm::mat4 Ryaw   = glm::mat4_cast(Qyaw);


    if (cmp.roll_lock)
    {
        Qroll = glm::quat(glm::vec3(0.0f));
        Rroll = glm::mat4(1.0f);
    }


    glm::mat4 R = glm::mat4_cast(cmp.rotation) * Ryaw * Rpitch * Rroll;
    glm::mat4 T = glm::translate(ident, cmp.position);
    glm::mat4 S = glm::scale(ident, (scale) ? cmp.scale * cmp.scale3 : glm::vec3(1.0f));

    return T * R * S;
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




idk::TransformCmp &
idk::TransformSys::getData( int obj_id )
{
    return idk::ECS2::getComponent<idk::TransformCmp>(obj_id);
}



void
idk::TransformSys::translateWorldspace( int obj_id, const glm::vec3 &v )
{
    glm::mat4 M = glm::inverse(getWorldMatrix(obj_id));
    getData(obj_id).position += glm::mat3(M) * v;
}


void
idk::TransformSys::translateLocalspace( int obj_id, const glm::vec3 &v )
{
    getData(obj_id).position += v;
}



glm::vec3
idk::TransformSys::getPositionLocalspace( int obj_id )
{
    return idk::ECS2::getComponent<idk::TransformCmp>(obj_id).position;
}



glm::vec3
idk::TransformSys::getPositionWorldspace( int obj_id )
{
    return glm::vec3(getModelMatrix(obj_id)[3]);
}


void
idk::TransformSys::setPositionWorldspace( int obj_id, const glm::vec3 &v )
{
    glm::vec3 delta = v - getPositionWorldspace(obj_id);
    translateWorldspace(obj_id, delta);
}

void
idk::TransformSys::setPositionLocalspace( int obj_id, const glm::vec3 &v )
{
    getData(obj_id).position = v;
}



glm::vec3
idk::TransformSys::getUp( int obj_id )
{
    return glm::normalize(glm::mat3(getModelMatrix(obj_id)) * glm::vec3(0.0f, 1.0f, 0.0f));
}


glm::vec3
idk::TransformSys::getRight( int obj_id )
{
    return glm::normalize(glm::mat3(getModelMatrix(obj_id)) * glm::vec3(1.0f, 0.0f, 0.0f));
}


glm::vec3
idk::TransformSys::getFront( int obj_id )
{
    return glm::normalize(glm::mat3(getModelMatrix(obj_id)) * glm::vec3(0.0f, 0.0f, -1.0f));
}


void
idk::TransformSys::setSurfaceUp( int obj_id, const glm::vec3 &up, float alpha )
{
    alpha = glm::clamp(alpha, 0.0f, 1.0f);
    getData(obj_id).up = glm::mix(getData(obj_id).up, up, alpha);
}


glm::vec3
idk::TransformSys::getSurfaceUp( int obj_id )
{
    return glm::normalize(getData(obj_id).up);
}


glm::vec3
idk::TransformSys::getSurfaceRight( int obj_id )
{
    return glm::normalize(glm::cross(getFront(obj_id), getSurfaceUp(obj_id)));
}


glm::vec3
idk::TransformSys::getSurfaceFront( int obj_id )
{
    return glm::normalize(glm::cross(getSurfaceUp(obj_id), getRight(obj_id)));
}




glm::mat4
idk::TransformSys::getLocalMatrix( int obj_id, bool scale )
{
    return getData(obj_id).local;
}


glm::mat4
idk::TransformSys::getWorldMatrix( int obj_id )
{
    return getData(obj_id).world;
}



glm::mat4
idk::TransformSys::getModelMatrix( int obj_id)
{
    return getData(obj_id).model;
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

    TransformSys::getData(subject).rotation = glm::normalize(glm::quat_cast(R));
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

    return getData(subject).yaw - delta;
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


    for (int i=0; i<3; i++)
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

    setPositionWorldspace(objC, posC);
    setPositionWorldspace(objB, posB);
    setPositionWorldspace(objA, posA);
}



void
idk::TransformSys::rotateX( int obj_id, float f )
{
    static const glm::vec3 axis = glm::vec3(1.0f, 0.0f, 0.0f);

    glm::quat &rot = getData(obj_id).rotation;
    rot = glm::rotate(rot, f, axis);
}


void
idk::TransformSys::rotateY( int obj_id, float f )
{
    static const glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::quat &rot = getData(obj_id).rotation;
    rot = glm::rotate(rot, f, axis);
}


void
idk::TransformSys::rotateZ( int obj_id, float f )
{
    static const glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f);

    glm::quat &rot = getData(obj_id).rotation;
    rot = glm::rotate(rot, f, axis);
}


void
idk::TransformSys::pitch( int obj_id, float f )
{
    float &pitch = getData(obj_id).pitch;
    pitch = glm::clamp(pitch + glm::radians(f), -1.45f, +1.45f);
}


void
idk::TransformSys::yaw( int obj_id, float f )
{
    getData(obj_id).yaw += glm::radians(f);
}


void
idk::TransformSys::roll( int obj_id, float f )
{
    getData(obj_id).roll += glm::radians(f);
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


void
idk::TransformSys::moveSurfaceUp( int obj_id, float f )
{
    glm::vec3 up = getSurfaceUp(obj_id);
    TransformSys::translateWorldspace(obj_id, f*up);
}


void
idk::TransformSys::moveSurfaceRight( int obj_id, float f )
{
    glm::vec3 right = getSurfaceRight(obj_id);
    TransformSys::translateWorldspace(obj_id, f*right);
}


void
idk::TransformSys::moveSurfaceFront( int obj_id, float f )
{
    glm::vec3 front = getSurfaceFront(obj_id);
    TransformSys::translateWorldspace(obj_id, f*front);
}

// ---------------------------------------------------------------------------------------------






size_t
idk::TransformCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, position);
    n += idk::streamwrite(stream, rotation);
    n += idk::streamwrite(stream, pitch);
    n += idk::streamwrite(stream, yaw);
    n += idk::streamwrite(stream, roll);
    n += idk::streamwrite(stream, up);
    n += idk::streamwrite(stream, front);
    n += idk::streamwrite(stream, right);
    n += idk::streamwrite(stream, scale);
    n += idk::streamwrite(stream, scale3);
    return n;
};


size_t
idk::TransformCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, position);
    n += idk::streamread(stream, rotation);
    n += idk::streamread(stream, pitch);
    n += idk::streamread(stream, yaw);
    n += idk::streamread(stream, roll);
    n += idk::streamread(stream, up);
    n += idk::streamread(stream, front);
    n += idk::streamread(stream, right);
    n += idk::streamread(stream, scale);
    n += idk::streamread(stream, scale3);
    return n;
};


void
idk::TransformCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = obj_id;
};


void
idk::TransformCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = -1;
};


void
idk::TransformCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<TransformCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<TransformCmp>(dst_obj);
    dst.obj_id = dst_obj;
};












size_t
idk::IKCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, pole_target);
    n += idk::streamwrite(stream, chain_length);
    return n;
};


size_t
idk::IKCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, pole_target);
    n += idk::streamread(stream, chain_length);
    return n;
};


void
idk::IKCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = obj_id;
};


void
idk::IKCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = -1;
};


void
idk::IKCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<TransformCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<TransformCmp>(dst_obj);
    dst.obj_id = dst_obj;
};














size_t
idk::LookTowardCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, target_id);
    return n;
};


size_t
idk::LookTowardCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, target_id);
    return n;
};


void
idk::LookTowardCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{

};


void
idk::LookTowardCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

};


void
idk::LookTowardCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<LookTowardCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<LookTowardCmp>(dst_obj);
    dst.obj_id = dst_obj;
};











size_t
idk::AnchorCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, anchor_ids);
    n += idk::streamwrite(stream, distances);
    return n;
};


size_t
idk::AnchorCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, anchor_ids);
    n += idk::streamread(stream, distances);
    return n;
};


void
idk::AnchorCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = obj_id;
};


void
idk::AnchorCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = -1;
};


void
idk::AnchorCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<AnchorCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<AnchorCmp>(dst_obj);
    dst.obj_id = dst_obj;
};








size_t
idk::SmoothFollowCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, anchor_id);
    n += idk::streamwrite(stream, speed);
    return n;
};


size_t
idk::SmoothFollowCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, anchor_id);
    n += idk::streamread(stream, speed);
    return n;
};


void
idk::SmoothFollowCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = obj_id;
};


void
idk::SmoothFollowCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = -1;
};


void
idk::SmoothFollowCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<SmoothFollowCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<SmoothFollowCmp>(dst_obj);
    dst.obj_id = dst_obj;
};


