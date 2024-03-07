#include "idk_systems.hpp"
#include "sys-transform.hpp"


static idk::EngineAPI *api_ptr;
static idk::ecs::ECS &getECS() { return api_ptr->getECS(); }


void
idk::TransformSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;
}



void
idk::TransformSys::update( idk::EngineAPI &api )
{
    auto &ecs   = api.getECS();
    auto &audio = api.getAudioSys();

    for (auto &cmp: ecs.getComponentArray<idk::TransformCmp>().getComponentAllocator())
    {
        recomputeTransformMatrices(cmp.obj_id);
    }

    _updateCallbacks();

}



void
idk::TransformSys::exposeToLua( lua_State *LS )
{
    luaaa::LuaModule mod(LS, "TransformSys");

    mod.fun("pitch", TransformSys::pitch);
    mod.fun("yaw",   TransformSys::yaw);
    mod.fun("roll",  TransformSys::roll);

    mod.fun("moveUp",    TransformSys::moveUp);
    mod.fun("moveRight", TransformSys::moveRight);
    mod.fun("moveFront", TransformSys::moveFront);

    mod.fun("moveSurfaceUp",    TransformSys::moveSurfaceUp);
    mod.fun("moveSurfaceRight", TransformSys::moveSurfaceRight);
    mod.fun("moveSurfaceFront", TransformSys::moveSurfaceFront);

    mod.fun("lookTowards", TransformSys::lookTowards);
    mod.fun("moveTowards", TransformSys::moveTowards);


    mod.fun("getXWorldspace", [](int id)
    {
        TransformSys::getPositionWorldspace(id).x;
    });

    mod.fun("getYWorldspace", [](int id)
    {
        TransformSys::getPositionWorldspace(id).y;
    });

    mod.fun("getZWorldspace", [](int id)
    {
        TransformSys::getPositionWorldspace(id).z;
    });

    mod.fun("setPositionWorldspace", [](int id, float x, float y, float z)
    {
        TransformSys::setPositionWorldspace(id, glm::vec3(x, y, z));
    });

    mod.fun("distance", [](int subject, int target)
    {
        return glm::distance(
            TransformSys::getPositionWorldspace(subject),
            TransformSys::getPositionWorldspace(target)
        );
    });

    mod.fun("isInsideBoundingRect", [](int subject, int target)
    {
        glm::vec3 position = getPositionWorldspace(subject);
        return TransformSys::isInsideBoundingRect(target, position);
    });


    mod.fun("copySurfaceUp", [](int from, int to)
    {
        glm::vec3 &up = getData(to).up;
        up = glm::mix(up, getUp(from), 0.05f);
    });


}




void
idk::TransformSys::recomputeTransformMatrices( int obj_id )
{
    cached_local[obj_id] = _computeLocalMatrix(obj_id, false);
    cached_world[obj_id] = _computeWorldMatrix(obj_id);
    cached_model[obj_id] = _computeModelMatrix(obj_id);
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


    glm::mat4 R = glm::inverse(glm::lookAt(glm::vec3(0.0f), cmp.front, cmp.up));
              R = glm::mat4_cast(cmp.rotation) * R * Ryaw * Rpitch * Rroll;

    glm::mat4 T = glm::translate(ident, cmp.position);
    glm::mat4 S = glm::scale(ident, (scale) ? cmp.scale * cmp.scale3 : glm::vec3(1.0f));

    return T * R * S;
}


glm::mat4
idk::TransformSys::_computeWorldMatrix( int obj_id )
{
    static const glm::mat4 ident = glm::mat4(1.0f);
    auto &ecs = getECS();

    if (obj_id == -1)
    {
        return ident;
    }

    int parent_id = ecs.getParent(obj_id);

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
    return getECS().getComponent<idk::TransformCmp>(obj_id);
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
    return getECS().getComponent<idk::TransformCmp>(obj_id).position;
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
    return _computeLocalMatrix(obj_id, scale);
}


glm::mat4
idk::TransformSys::getWorldMatrix( int obj_id )
{
    return cached_world[obj_id];
}



glm::mat4
idk::TransformSys::getModelMatrix( int obj_id)
{
    return cached_model[obj_id];
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



float
idk::TransformSys::lookTowards( int subject, int target, float alpha )
{
    glm::vec3 dir = getPositionWorldspace(target) - getPositionWorldspace(subject);
              dir = glm::normalize(dir);

    glm::vec3 front = getFront(subject);
    glm::vec3 up    = getUp(subject);

    float dot   = glm::dot(dir, front);
    float det   = glm::dot(up, glm::cross(dir, front));
    float delta = atan2(det, dot);

    if (fabs(delta) > 3.14f)
    {
        delta -= glm::sign(delta) * M_PI;
    }

    TransformSys::yaw(subject, alpha*delta);

    return getData(subject).yaw - delta;
}


float
idk::TransformSys::moveTowards( int subject, int target, float alpha )
{
    glm::vec3 pos_A = getPositionWorldspace(target);
    glm::vec3 pos_B = getPositionWorldspace(subject);
    glm::vec3 dir = glm::normalize(pos_A - pos_B);

    glm::vec3 up    = getSurfaceUp(subject);
    glm::vec3 right = getSurfaceRight(subject);

    right = glm::cross(dir, up);
    dir = glm::cross(up, right);


    translateWorldspace(subject, alpha * dir);

    return getDistanceWorldspace(subject, target);
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
    float &y = getData(obj_id).yaw;
    y += glm::radians(f);
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
    // up = glm::vec3(0.0f, 1.0f, 0.0f);
    // right = glm::vec3(1.0f, 0.0f, 0.0f);
    // front = glm::vec3(0.0f, 0.0f, -1.0f);

    return n;
};


void
idk::TransformCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    this->obj_id = obj_id;
};


void
idk::TransformCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    this->obj_id = -1;
};


void
idk::TransformCmp::onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
{
    auto &src = api.getECS().getComponent<TransformCmp>(src_obj);

    *this = src;
    this->obj_id = dst_obj;
};


