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
    
    for (auto &cmp: ECS2::getComponentArray<ScriptCmp>())
    {
        if (cmp.script.is_ready())
        {
            cmp.script.execute(api, cmp.obj_id, cmp.obj_id);
        }
    }
}


void
idk::ScriptSys::assignScript( int obj_id, const std::string &filepath )
{
    if (ECS2::hasComponent<ScriptCmp>(obj_id) == false)
    {
        ECS2::giveComponent<ScriptCmp>(obj_id);
    }

    auto &cmp = ECS2::getComponent<ScriptCmp>(obj_id);
    cmp.filepath = filepath;
    cmp.script   = idk::RuntimeScript(filepath);
}



size_t
idk::ScriptCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, filepath);
    return n;
}


size_t
idk::ScriptCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, filepath);
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
    
    // dst.model_id = src.model_id;
    dst.filepath = src.filepath;
    dst.script   = src.script;
}


