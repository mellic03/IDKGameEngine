#include "idk_events.hpp"

#include <libidk/idk_allocator.hpp>


struct EventWrapper
{
    int                   obj_id;
    std::function<bool()> trigger;
    std::function<void()> callback;
    std::vector<int>      children;
};


namespace
{
    idk::Allocator<EventWrapper> m_events;
}




int
idk::Events::createEvent( std::function<bool()> trigger, std::function<void()> callback )
{
    EventWrapper e;
    e.trigger  = trigger;
    e.callback = callback;

    int id = m_events.create(e);
    m_events.get(id).obj_id = id;

    return id;
}


int
idk::Events::createEvent( std::function<bool()> trigger, std::function<void()> callback, int dep )
{
    EventWrapper e;
    e.trigger  = trigger;
    e.callback = callback;

    int id = m_events.create(e);
    m_events.get(id).obj_id = id;

    if (m_events.contains(dep))
    {
        m_events.get(dep).children.push_back(id);
    }

    return id;
}



void
idk::Events::update()
{
    static std::vector<int> cull;
    cull.clear();

    for (EventWrapper &e: m_events)
    {
        if (e.trigger())
        {
            e.callback();
            cull.push_back(e.obj_id);
        }
    }

    for (int obj_id: cull)
    {
        m_events.destroy(obj_id);
    }

}


