#include "sys-script.hpp"
#include <libidk/idk_log.hpp>

static idk::EngineAPI *api_ptr;


void
idk::ScriptSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;

    // for (auto &cmp: ecs.getComponentArray<idk::ScriptCmp>())
    // {
    //     if (cmp.filepath[0] != '\0')
    //     {
    //         cmp.model_id = ren.loadModel(cmp.filepath);
    //     }
    // }
}



void
idk::ScriptSys::update( idk::EngineAPI &api )
{
    api_ptr = &api;

    auto &ecs = getECS();
    auto &ren = api.getRenderer();
    float dt  = api.dtime();
    
    for (auto &cmp: ecs.getComponentArray<ScriptCmp>())
    {
        for (int i=0; i<cmp.scripts.size(); i++)
        {
            if (cmp.scripts[i] == "" || cmp.autoexec[i] == false)
            {
                continue;
            }

            cmp.timers[i] -= int(1000.0f * dt);

            if (cmp.timers[i] <= 0)
            {
                void *data    = cmp.data[i];
                auto *script  = m_scripts[cmp.scripts[i]];
                cmp.timers[i] = script->execute(api, data);
            }
        }
    }
}




// void
// idk::ScriptSys::reserve( int obj_id, int count )
// {
//     auto &ecs = getECS();
//     auto &cmp = ecs.getComponent<ScriptCmp>(obj_id);

//     cmp.data.resize(count, nullptr);
//     cmp.scripts.resize(count);
//     cmp.timers.resize(count, 0);
// }


void
idk::ScriptSys::reloadScript( const std::string &filename )
{
    if (m_scripts.contains(filename))
    {
        LOG_INFO("Reloading script: \"{}\"", filename);
        m_scripts[filename]->reload();
    }
}


int
idk::ScriptSys::attachScript( int obj_id, const std::string &filename, void *data, bool autoexec )
{
    namespace fs = std::filesystem;
    auto &ecs = getECS();

    if (m_scripts.contains(filename) == false)
    {
        std::cout << "New script: \"" << filename << "\"\n";
        m_scripts[filename] = new idk::RuntimeScript(filename);
    }

    if (ecs.hasComponent<ScriptCmp>(obj_id) == false)
    {
        ecs.giveComponent<ScriptCmp>(obj_id);
    }

    auto &cmp = ecs.getComponent<ScriptCmp>(obj_id);
    cmp.data.push_back(data);
    cmp.scripts.push_back(filename);
    cmp.timers.push_back(0);
    cmp.autoexec.push_back(autoexec);

    return cmp.data.size() - 1;
}


void
idk::ScriptSys::removeScript( int obj_id, const std::string &name )
{
    namespace fs = std::filesystem;

    auto &ecs = getECS();
    auto &cmp = ecs.getComponent<ScriptCmp>(obj_id);

    for (int i=0; i<cmp.scripts.size(); i++)
    {
        if (cmp.scripts[i] == name)
        {
            cmp.data[i]    = nullptr;
            cmp.scripts[i] = "";
        }
    }
}


void
idk::ScriptSys::executeScript( int obj_id, const std::string &name, void *data )
{
    auto &ecs = getECS();
    auto &cmp = ecs.getComponent<ScriptCmp>(obj_id);

    for (int i=0; i<cmp.scripts.size(); i++)
    {
        if (cmp.scripts[i] == name)
        {
            m_scripts[name]->execute(*api_ptr, data);
            return;
        }
    }
}




void
idk::ScriptSys::attachData( int obj_id, int idx, void *data )
{
    auto &ecs = getECS();
    ecs.getComponent<ScriptCmp>(obj_id).data[idx] = data;
}



idk::RuntimeScript*
idk::ScriptSys::getScript( const std::string &name )
{
    if (m_scripts.contains(name) == false)
    {
        return nullptr;
    }
    return m_scripts[name];
}




size_t
idk::ScriptCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, scripts);
    return n;
}


size_t
idk::ScriptCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, scripts);

    for (auto &script: scripts)
    {
        script = "";
    }

    return n;
}


void
idk::ScriptCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    ScriptCmp &cmp = ecs.getComponent<ScriptCmp>(obj_id);

    // if (cmp.model_id == -1 && cmp.filepath != "")
    // {
    //     cmp.model_id = api.getRenderer().loadModel(cmp.filepath);
    // }
}


void
idk::ScriptCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{

}


void
idk::ScriptCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
{
    ScriptCmp &src = ecs.getComponent<ScriptCmp>(src_obj);
    ScriptCmp &dst = ecs.getComponent<ScriptCmp>(dst_obj);
    
    dst.data    = src.data;
    dst.scripts = src.scripts;
}


