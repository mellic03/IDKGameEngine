#include "sys-trigger.hpp"

#include <libidk/idk_geometry.hpp>


namespace
{
    std::map<std::string, idk::TriggerSys::trigger_type>  m_triggers;
    std::map<std::string, idk::TriggerSys::callback_type> m_callbacks;
}


void
idk::TriggerSys::init( idk::EngineAPI &api )
{
    m_triggers[""]  = [](idk::EngineAPI&, int){ return false; };
    m_callbacks[""] = [](idk::EngineAPI&, int){};
}


void
idk::TriggerSys::update( idk::EngineAPI &api )
{
    auto &ecs = getECS();

    for (auto &[obj_id, trigger, response]: ecs.getComponentArray<TriggerCmp>())
    {
        if (m_triggers.contains(trigger) == false)
        {
            LOG_ERROR() << "Trigger \"" << trigger << "\" does not exist!\n";
            return;
        }

        if (m_callbacks.contains(response) == false)
        {
            LOG_ERROR() << "Callback \"" << response << "\" does not exist!\n";
            return;
        }

        if (m_triggers[trigger](api, obj_id))
        {
            m_callbacks[response](api, obj_id);
        }
    }
}



void
idk::TriggerSys::registerTrigger( const std::string &name, trigger_type trigger )
{
    m_triggers[name] = trigger;
}


void
idk::TriggerSys::registerResponse( const std::string &name, callback_type callback )
{
    m_callbacks[name] = callback;
}








size_t
idk::TriggerCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, trigger);
    n += idk::streamwrite(stream, response);
    return n;
};


size_t
idk::TriggerCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, trigger);
    n += idk::streamread(stream, response);
    return n;
};


void
idk::TriggerCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{

};


void
idk::TriggerCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{

};


void
idk::TriggerCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
{
    auto &src = ecs.getComponent<TriggerCmp>(src_obj);
    auto &dst = ecs.getComponent<TriggerCmp>(dst_obj);

};
