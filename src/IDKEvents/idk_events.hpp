#pragma once

#include <functional>


namespace idk::Events
{
    int createEvent( std::function<bool()>, std::function<void()> );
    int createEvent( std::function<bool()>, std::function<void()>, int dep );

    void update();
}


