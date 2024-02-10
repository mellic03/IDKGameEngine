#include "idk_systems.hpp"



static idk::EngineAPI *api_ptr;
static idecs::ECS &getECS() { return api_ptr->getECS(); }


void
idk::TransformSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;
}


// struct lua_vec3
// {
//     float x, y, z;

//     float get_x() const { return x; };
//     float get_y() const { return y; };
//     float get_z() const { return z; };

//     void set_x(float _x) { x = _x; };
//     void set_y(float _y) { y = _y; };
//     void set_z(float _z) { z = _z; };

//     lua_vec3() {  };
//     lua_vec3(float _x, float _y, float _z): x(_x), y(_y), z(_z) {  };

// };



void
idk::TransformSys::exposeToLua( lua_State *LS )
{
    // luaaa::LuaClass<lua_vec3> vec3(LS, "vec3");
    // vec3.ctor<float, float, float>("new");

    // vec3.set("x", &lua_vec3::set_x);
    // vec3.set("y", &lua_vec3::set_y);
    // vec3.set("z", &lua_vec3::set_z);

    // vec3.get("x", &lua_vec3::get_x);
    // vec3.get("y", &lua_vec3::get_y);
    // vec3.get("z", &lua_vec3::get_z);


    luaaa::LuaModule mod(LS, "TransformSys");

    mod.fun("pitch",     [](int id, float f) { TransformSys::pitch(id, f); });
    mod.fun("yaw",       [](int id, float f) { TransformSys::yaw  (id, f); });
    mod.fun("yawGlobal", [](int id, float f) { TransformSys::yaw_global(id, f); });

    mod.fun("lookTowards", [](int subject, int target, float alpha)
    {
        TransformSys::lookTowards(subject, target, alpha);
    });

    mod.fun("translate", [](int id, float x, float y, float z)
    {
        TransformSys::translate(id, glm::vec3(x, y, z));
    });

    mod.fun("translateLocal", [](int id, float x, float y, float z)
    {
        TransformSys::translate_local(id, glm::vec3(x, y, -z));
    });

    mod.fun("distance", [](int subject, int target)
    {
        return glm::distance(
            TransformSys::getPosition(subject),
            TransformSys::getPosition(target)
        );
    });

}



idk::TransformCmp &
idk::TransformSys::getData( int obj_id )
{
    return getECS().getComponent<idk::TransformCmp>(obj_id);
}


void
idk::TransformSys::translate( int obj_id, const glm::vec3 &v )
{
    getPosition(obj_id) += v;
}


void
idk::TransformSys::translate_local( int obj_id, const glm::vec3 &v )
{
    getPosition(obj_id) += getData(obj_id).rotation * v;
}




glm::vec3 &
idk::TransformSys::getPosition( int obj_id )
{
    return getECS().getComponent<idk::TransformCmp>(obj_id).position;
}



glm::vec3
idk::TransformSys::getWorldPosition( int obj_id )
{
    return glm::vec3(getWorldMatrix(obj_id)[3]);
}


glm::vec3
idk::TransformSys::getFront( int obj_id )
{
    auto &cmp = getECS().getComponent<TransformCmp>(obj_id);
    return cmp.rotation * glm::vec3(0.0f, 0.0f, -1.0f);
}


glm::mat4
idk::TransformSys::getModelMatrix( int obj_id, bool scale )
{
    static const glm::mat4 ident = glm::mat4(1.0f);

    if (obj_id == -1)
    {
        return ident;
    }

    auto &data = getData(obj_id);

    glm::mat4 T = glm::translate(ident, data.position);
    glm::mat4 R = glm::mat4_cast(data.rotation);
    glm::mat4 S = glm::scale(ident, (scale) ? data.scale : glm::vec3(1.0f));

    return T * R * S;
};


glm::mat4
idk::TransformSys::getWorldMatrix( int obj_id )
{
    static const glm::mat4 ident = glm::mat4(1.0f);
    auto &ecs = getECS();

    if (obj_id == -1)
    {
        return ident;
    }

    if (ecs.hasComponent<TransformCmp>(obj_id) == false)
    {
        return ident;
    }

    glm::vec3 position = getData(obj_id).position;
    glm::quat rotation = getData(obj_id).rotation;
    glm::vec3 scale    = getData(obj_id).scale;
    obj_id = ecs.getParent(obj_id);

    while (obj_id != -1)
    {
        if (ecs.hasComponent<TransformCmp>(obj_id))
        {
            glm::vec3 parent_pos = getData(obj_id).position;
            glm::quat parent_rot = getData(obj_id).rotation;
            glm::vec3 parent_scl = getData(obj_id).scale;

            position = parent_pos + (parent_rot * position);
            rotation = parent_rot * rotation;
        }

        obj_id = ecs.getParent(obj_id);
    }

    glm::mat4 T = glm::translate(ident, position);
    glm::mat4 R = glm::mat4_cast(rotation);
    glm::mat4 S = glm::scale(ident, scale);

    return T * R * S;
};


void
idk::TransformSys::lookTowards( int subject, int target, float alpha )
{
    auto &A_data = getData(subject);
    auto &B_data = getData(target);


    glm::vec3 dir = glm::normalize(B_data.position - A_data.position);
    glm::quat R = glm::quatLookAt(dir, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::quat &Q = getData(subject).rotation;
    Q = glm::slerp(Q, R, alpha);
}



void
idk::TransformSys::pitch( int obj_id, float f )
{
    static glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    static glm::vec3 axis;
    static glm::quat delta;

    glm::quat &rotation = getData(obj_id).rotation;

    axis = rotation * right;
    delta = glm::angleAxis(f, axis);
    rotation = delta * rotation;
}


void
idk::TransformSys::yaw( int obj_id, float f )
{
    static glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    static glm::vec3 axis;
    static glm::quat delta;

    glm::quat &rotation = getData(obj_id).rotation;

    axis = rotation * up;
    delta = glm::angleAxis(f, axis);
    rotation = delta * rotation;
}


void
idk::TransformSys::yaw_global( int obj_id, float f )
{
    static glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
    static glm::quat delta;

    glm::quat &rotation = getData(obj_id).rotation;

    delta = glm::angleAxis(f, axis);
    rotation = delta * rotation;
}