#include "sys-script.hpp"

static idk::EngineAPI *api_ptr;


void
idk::ScriptSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;

    // for (auto &cmp: ECS2::getComponentArray<idk::ScriptCmp>())
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
    auto &ren = api.getRenderer();
    float dt  = api.dtime();
    
    for (auto &cmp: ECS2::getComponentArray<ScriptCmp>())
    {
        for (int i=0; i<cmp.scripts.size(); i++)
        {
            if (cmp.scripts[i] == "")
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




void
idk::ScriptSys::reserve( int obj_id, int count )
{
    auto &cmp = ECS2::getComponent<ScriptCmp>(obj_id);

    cmp.data.resize(count, nullptr);
    cmp.scripts.resize(count);
    cmp.timers.resize(count, 0);
}


void
idk::ScriptSys::reloadScript( const std::string &filename )
{
    if (m_scripts.contains(filename))
    {
        m_scripts[filename]->reload();
    }
}


void
idk::ScriptSys::attachScript( int obj_id, int idx, const std::string &filename )
{
    namespace fs = std::filesystem;

    if (ECS2::hasComponent<ScriptCmp>(obj_id) == false)
    {
        ECS2::giveComponent<ScriptCmp>(obj_id);
    }

    if (m_scripts.contains(filename) == false)
    {
        std::cout << "New script: \"" << filename << "\"\n";
        m_scripts[filename] = new idk::RuntimeScript(filename);
    }

    auto &cmp = ECS2::getComponent<ScriptCmp>(obj_id);
    cmp.scripts[idx] = filename;
}



void
idk::ScriptSys::attachData( int obj_id, int idx, void *data )
{
    ECS2::getComponent<ScriptCmp>(obj_id).data[idx] = data;
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
idk::ScriptCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    ScriptCmp &cmp = ECS2::getComponent<ScriptCmp>(obj_id);

    // if (cmp.model_id == -1 && cmp.filepath != "")
    // {
    //     cmp.model_id = api.getRenderer().loadModel(cmp.filepath);
    // }
}


void
idk::ScriptCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

}


void
idk::ScriptCmp::onObjectCopy( int src_obj, int dst_obj )
{
    ScriptCmp &src = idk::ECS2::getComponent<ScriptCmp>(src_obj);
    ScriptCmp &dst = idk::ECS2::getComponent<ScriptCmp>(dst_obj);
    
    dst.data    = src.data;
    dst.scripts = src.scripts;
}


