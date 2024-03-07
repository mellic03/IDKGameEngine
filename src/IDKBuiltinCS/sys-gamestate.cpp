#include "sys-gamestate.hpp"
#include "sys-player.hpp"

#include "idk_systems.hpp"


static idk::EngineAPI *api_ptr;
static idk::EngineAPI &getAPI() { return *api_ptr; };
static idk::ecs::ECS  &getECS() { return api_ptr->getECS(); };



void
idk::GameStateSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;
}


void
idk::GameStateSys::update( idk::EngineAPI &api )
{
    auto &ecs = api.getECS();

    for (auto &cmp: ecs.getComponentArray<idk::ProgressionEventCmp>())
    {
        if (cmp.state_id == -1 || cmp.script_id == -1)
        {
            continue;
        }

        auto &state  = _getProgressionState(cmp.state_id);
        auto &script = ecs.getComponent<ScriptCmp>(cmp.script_id);


        if (state.progress == cmp.prereq_progress)
        {
            script.enabled = true;

            if (script.retvalue > 0)
            {
                state.progress += 1;
            }
        }

        else
        {
            script.enabled = false;
        }

    }

}


void
idk::GameStateSys::exposeToLua( lua_State *LS )
{
    luaaa::LuaModule mod(LS, "GameStateSys");

    mod.fun("incrementProgress", [](int id) { GameStateSys::_getProgressionState(id).progress += 1; });

}


idk::ProgressionEventCmp &
idk::GameStateSys::_getProgressionEvent( int obj_id )
{
    return getECS().getComponent<idk::ProgressionEventCmp>(obj_id);
}



idk::ProgressionStateCmp &
idk::GameStateSys::_getProgressionState( int obj_id )
{
    return getECS().getComponent<idk::ProgressionStateCmp>(obj_id);
}



void
idk::GameStateSys::assignScript( int obj_id, int script )
{
    _getProgressionEvent(obj_id).script_id = script;
}

