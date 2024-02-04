#include "idk_systems.hpp"
#include <IDKEvents/IDKEvents.hpp>

#include <filesystem>
namespace fs = std::filesystem;


static idk::EngineAPI *api_ptr;
static idecs::ECS &getECS() { return api_ptr->getECS(); }



void
idk::PhysicsSys::update( idk::EngineAPI &api )
{
    auto &ecs = api.getECS();
    auto &arr = ecs.getComponentArray<idk::PhysicsMotionCmp>().data();

    for (idk::PhysicsMotionCmp &cmp: arr)
    {
        IDK_ASSERT("PhysicsMotionCmp::obj_id == -1", cmp.obj_id != -1);

        auto &transcmp = ecs.getComponent<idk::TransformCmp>(cmp.obj_id);

        cmp.lin_velocity *= cmp.lin_drag;
        cmp.ang_velocity *= cmp.ang_drag;

        transcmp.position += cmp.lin_velocity;

        glm::vec3 axis  = glm::normalize(cmp.ang_axis);
        glm::quat delta = glm::quat_cast(glm::rotate(glm::mat4(1.0f), cmp.ang_velocity, axis));
        transcmp.rotation = delta * transcmp.rotation;
    }


    // for (idk::AngularVelocityCmp &cmp: av_arr)
    // {
    //     IDK_ASSERT("VelocityCmp::obj_id == -1", cmp.obj_id != -1);

    //     auto &transcmp = ecs.getComponent<idk::TransformCmp>(cmp.obj_id);

    //     glm::quat &rotation = transcmp.rotation;

    //     glm::vec3 axis  = glm::normalize(cmp.axis);
    //     glm::quat delta = glm::quat_cast(glm::rotate(glm::mat4(1.0f), cmp.magnitude, axis));

    //     rotation = delta * rotation;

    //     cmp.magnitude *= cmp.drag;

    // }
}



void
idk::ModelSys::init( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    auto &ecs = getECS();
    auto &arr = ecs.getComponentArray<idk::ModelCmp>().data();

    for (auto &cmp: arr)
    {
        if (cmp.filepath[0] != '\0')
        {
            cmp.model_id = ren.modelSystem().loadModel(cmp.filepath, cmp.filestem);
        }
    }
}


void
idk::ModelSys::update( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    auto &ecs = getECS();
    auto &arr = ecs.getComponentArray<idk::ModelCmp>().data();

    for (idk::ModelCmp &data: arr)
    {
        if (data.model_id == -1 || data.visible == false)
        {
            continue;
        }

        glm::mat4 transform = TransformSys::getWorldMatrix(data.obj_id);

        ren.drawModel(data.model_id, transform);
        ren.drawEnvironmental(data.model_id, transform);

        if (data.shadowcast)
        {
            ren.drawShadowCaster(data.model_id, transform);
        }
    }
}


void
idk::ModelSys::assignModel( int obj_id, int model_id )
{
    auto &ecs = getECS();
    auto &cmp = ecs.getComponent<idk::ModelCmp>(obj_id);
    ecs.giveComponent<idk::ModelCmp>(obj_id);

    idk::Model &model = api_ptr->getRenderer().modelSystem().getModel(model_id);

    cmp.obj_id = obj_id;
    cmp.model_id = model_id;
    std::strcpy(cmp.filepath, model.filepath.c_str());
    std::strcpy(cmp.filestem, model.filestem.c_str());

}




void
idk::ScriptSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;

    static auto &apiref = api;
    static auto &engine = api.getEngine();
    static auto &events = api.getEventSys();

    auto &v = getECS().getComponentArray<idk::ScriptCmp>().data();
    for (auto &data: v)
    {
        data = idk::ScriptCmp(data.obj_id, &data);
    }
}



void
idk::ScriptSys::update( idk::EngineAPI &api )
{
    auto &ecs = api.getECS();
    auto &component_array = ecs.getComponentArray<idk::ScriptCmp>().data();

    for (auto &data: component_array)
    {
        auto &L        = data.L;
        auto &nparams  = data.nparams;
        auto &subject  = data.subject_id;
        auto &target   = data.target_id;
        int  dep       = data.dependency;
        auto &filepath = data.filepath;

        IDK_ASSERT("null lua_State!", L != nullptr);


        if (dep != -1)
        {
            if (ecs.hasComponent<ScriptCmp>(dep) == false)
            {
                return;
            }

            if (ecs.getComponent<ScriptCmp>(dep).retvalue != 1)
            {
                return;
            }
        }

        if (std::strcmp(filepath, "") == 0)
        {
            continue;
        }

        if (luaL_dofile(L, filepath) != 0)
        {
            std::cerr << "Error loading Lua script: " << lua_tostring(L, -1) << std::endl;
        }

        lua_getglobal(L, "ScriptEntry");

        if (lua_isfunction(L, -1) == false)
        {
            std::cerr << "Error: 'ScriptEntry' is not a Lua function" << std::endl;
            lua_pop(L, 1);
        }

        (subject == -1) ? lua_pushnil(L) : lua_pushinteger(L, subject);
        (target  == -1) ? lua_pushnil(L) : lua_pushinteger(L, target);

        if (nparams == 2 && target == -1)
        {
            return;
        }

        if (lua_pcall(L, 2, 1, 0) != 0)
        {
            std::cerr << "Error calling Lua function: " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);
        }

        data.retvalue = lua_tointeger(L, -1);
        lua_pop(L, 1);

        // idk_printvalue(ret);
    }

}


void
idk::ScriptSys::loadScript( const std::string &filepath )
{
    m_scripts.push_back(filepath);
}


void
idk::ScriptSys::loadScripts( const std::string &directory )
{
    namespace fs = std::filesystem;

    for (auto dir_iter: fs::directory_iterator(directory))
    {
        if (dir_iter.is_directory())
        {
            continue;
        }

        auto path = dir_iter.path();

        if (path.has_extension() && path.extension() == ".lua")
        {
            loadScript(path.string());
        }
    }
}


void
idk::ScriptSys::assignScript( int obj_id, const std::string &filepath )
{
    auto &ecs = getECS();

    int child = ecs.createGameObject(fs::path(filepath).stem());
    ecs.giveChild(obj_id, child);

    ecs.giveComponent<idk::ScriptCmp>(child);
    auto &cmp = ecs.getComponent<idk::ScriptCmp>(child);

    std::strcpy(cmp.filepath, filepath.c_str());
    cmp.nparams = idk::ScriptCmp_getNumParams(cmp);

    auto &icon = ecs.getComponent<idk::IconCmp>(child).icon;
    auto  name = ICON_FA_FILE_CODE;
    std::strcpy(icon, name);
}




void
idk::CameraSys::update( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    auto &ecs = api.getECS();

    for (auto &[obj_id, cam_id]: ecs.getComponentArray<idk::CameraCmp>().data())
    {
        if (obj_id == -1 || cam_id == -1)
        {
            continue;
        }

        ren.getCamera(cam_id).model() = TransformSys::getWorldMatrix(obj_id);

    }

}


bool
idk::CameraSys::in_frustum( int subject, int target )
{
    auto &ren = api_ptr->getRenderer();
    auto &ecs = api_ptr->getECS();
    auto &cmp = ecs.getComponent<idk::CameraCmp>(target);

    idk::Camera &cam = ren.getCamera(cmp.cam_id);


    return false;
}

