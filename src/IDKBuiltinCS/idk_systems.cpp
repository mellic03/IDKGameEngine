#include "idk_systems.hpp"
#include <IDKEvents/IDKEvents.hpp>

#include "sys-transform.hpp"
#include "sys-planet.hpp"


#include <filesystem>
namespace fs = std::filesystem;


static idk::EngineAPI *api_ptr;
static idk::ecs::ECS &getECS() { return api_ptr->getECS(); }




void
idk::ModelSys::init( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    auto &ecs = getECS();
    auto &arr = ecs.getComponentArray<idk::ModelCmp>();

    for (auto &cmp: arr)
    {
        if (cmp.filepath[0] != '\0')
        {
            cmp.model_id = ren.loadModel(cmp.filepath);
        }
    }
}


void
idk::ModelSys::update( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    auto &ecs = getECS();

    for (idk::ModelCmp &cmp: ecs.getComponentArray<idk::ModelCmp>())
    {
        if (cmp.model_id == -1 || cmp.visible == false)
        {
            continue;
        }

        glm::mat4 transform = TransformSys::getModelMatrix(cmp.obj_id);


        if (cmp.shader_enabled && cmp.shader_name != "")
        {
            
        }

        else
        {
            ren.drawModel(cmp.model_id, transform);
        }

        if (cmp.shadowcast)
        {
            ren.drawShadowCaster(cmp.model_id, transform);
        }
    }
}


void
idk::ModelSys::assignModel( int obj_id, const std::string &filepath )
{
    auto &ecs = getECS();
    auto &cmp = ecs.getComponent<idk::ModelCmp>(obj_id);

    int model_id = api_ptr->getRenderer().loadModel(filepath);

    cmp.obj_id   = obj_id;
    cmp.model_id = model_id;
    cmp.filepath = filepath;
}


void
idk::ModelSys::assignShader_gpass( int obj_id, const std::string &shader_name )
{
    auto &ecs  = getECS();
    auto &cmp  = ecs.getComponent<idk::ModelCmp>(obj_id);
    cmp.shader_name = shader_name;
}




void
idk::ScriptSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;

    static auto &apiref = api;
    static auto &engine = api.getEngine();
    static auto &events = api.getEventSys();

}



void
idk::ScriptSys::update( idk::EngineAPI &api )
{
    auto &ecs = api.getECS();
    auto &component_array = ecs.getComponentArray<idk::ScriptCmp>();

    for (auto &cmp: component_array)
    {
        auto &L        = cmp.L;
        auto &nparams  = cmp.nparams;
        auto &subject  = cmp.subject_id;
        auto &target   = cmp.target_id;
        int  dep       = cmp.dependency;
        auto &filepath = cmp.filepath;


        if (cmp.enabled == false)
        {
            continue;
        }

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

        if (filepath == "")
        {
            continue;
        }

        if (luaL_dofile(L, filepath.c_str()) != 0)
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

        cmp.retvalue = lua_tointeger(L, -1);
        lua_pop(L, 1);
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


int
idk::ScriptSys::assignScript( int obj_id, const std::string &filepath )
{
    auto &ecs = getECS();

    int child = ecs.createGameObject(fs::path(filepath).stem());
    ecs.giveChild(obj_id, child);

    auto &cmp = ecs.getComponent<idk::ScriptCmp>(child);

    cmp.filepath = filepath;
    cmp.nparams = idk::ScriptCmp_getNumParams(cmp);

    auto &icon = ecs.getComponent<idk::IconCmp>(child).icon;
          icon = ICON_FA_FILE_CODE;

    return child;
}




void
idk::CameraSys::exposeToLua( lua_State *LS )
{
    luaaa::LuaModule mod(LS, "CameraSys");

}



void
idk::CameraSys::update( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &events = api.getEventSys();
    auto &ren    = api.getRenderer();
    auto &ecs    = api.getECS();

    float dtime = engine.deltaTime();

    for (auto &cmp: ecs.getComponentArray<idk::CameraCmp>())
    {
        int obj_id = cmp.obj_id;
        int cam_id = cmp.cam_id;

        if (cam_id == -1)
        {
            continue;
        }

        IDK_ASSERT("Object does not exist", obj_id >= 0);
        IDK_ASSERT("Camera does not exist", cam_id >= 0);

        auto &camera = ren.getCamera(cam_id);
        camera.bloom = cmp.bloom;

        camera.setModelMatrix(TransformSys::getModelMatrix(obj_id));


        int planet = PlanetSys::nearestPlanet(camera.position);

        if (planet == -1)
        {
            continue;
        }


        glm::vec3 pos = TransformSys::getPositionWorldspace(planet);
        glm::vec3 dir = glm::normalize(pos);

        ren.lightSystem().getDirlight(0).direction = glm::vec4(dir, 0.0f);
        ren.lightSystem().getDirlight(0).ambient   = glm::vec4(0.04f);
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



void
idk::LightSys::init( idk::EngineAPI &api )
{

}


void
idk::LightSys::update( idk::EngineAPI &api )
{
    auto &ecs = api.getECS();
    auto &ren = api.getRenderer();

    for (auto &[obj_id, light_id, diffuse, radius]: ecs.getComponentArray<idk::PointlightCmp>())
    {
        auto &light = ren.getPointlight(light_id);
    
        glm::mat4 M    = TransformSys::getModelMatrix(obj_id);
        light.position = M[3];
        light.diffuse  = diffuse;
        light.radius   = radius;
    }

    for (auto &[obj_id, light_id, diffuse, angle, radius]: ecs.getComponentArray<idk::SpotlightCmp>())
    {
        auto &light = ren.getSpotlight(light_id);

        glm::mat4 M = TransformSys::getModelMatrix(obj_id);
        light.position = M[3];
        light.orientation = glm::normalize(glm::quat_cast(M));

        light.diffuse = diffuse;
        light.angle   = angle;
        light.radius  = radius;
    }

}

