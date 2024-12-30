#include "./packager.hpp"


void
idk::Packager::onBuild( const std::function<void()> &callback )
{
    m_callbacks.push_back(callback);
}


int
idk::Packager::build()
{
    for (auto &callback: m_callbacks)
    {
        callback();
    }

    return 0;
}

