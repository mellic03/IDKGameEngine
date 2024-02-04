#include "idk_components.hpp"


int
idk::ScriptCmp_getNumParams( idk::ScriptCmp &cmp )
{
    lua_State *L = cmp.L;

    if (luaL_dofile(L, cmp.filepath) != 0)
    {
        std::cerr << "Error loading Lua script: " << lua_tostring(L, -1) << std::endl;
    }

    lua_getglobal(L, "ScriptEntry");

    if (lua_isfunction(L, -1) == false)
    {
        std::cerr << "Error: 'ScriptEntry' is not a Lua function" << std::endl;
        lua_pop(L, 1);
    }

    lua_Debug ar;
    lua_pushvalue(L, -1);
    lua_getinfo(L, ">u", &ar);
    std::cout <<  int(ar.nparams) << "\n";

    return int(ar.nparams);
}



void
idk::registerComponents( idecs::ECS &ecs )
{
    ecs.registerComponentMandatory<idk::IconCmp>("Icon");
    ecs.registerComponent<TransformCmp>         ("Transform");
    ecs.registerComponent<PhysicsMotionCmp>     ("PhysicsMotion");
    ecs.registerComponent<BoxColliderCmp>       ("BoxCollider");
    ecs.registerComponent<SphereColliderCmp>    ("SphereCollider");
    ecs.registerComponent<ModelCmp>             ("Model");
    ecs.registerComponent<ScriptCmp>            ("Script");
    ecs.registerComponent<CameraCmp>            ("Camera");

    ecs.setComponentBehaviour<idk::TransformCmp>  (idk::getBehaviour<idk::TransformCmp>());
    ecs.setComponentBehaviour<idk::ModelCmp>      (idk::getBehaviour<idk::ModelCmp>());
}




// void
// idk::SomeSystem::update( idk::EngineAPI &api )
// {
//     int obj_id = api.getEngine().getSelectedGameObject();
//     std::cout << "selected: " << obj_id << "\n";
// }
