#include "idk_lightsystem.hpp"


void
idk::LightSystem::init()
{
    m_dirlight_config = {
        .internalformat = GL_RGBA16F,
        .minfilter      = GL_NEAREST,
        .magfilter      = GL_NEAREST,
        .datatype       = GL_FLOAT
    };

}


int
idk::LightSystem::createLightsource( idk::lightsource type )
{
    m_changed = true;

    switch (type)
    {
        case lightsource::POINT:
            m_pointlights.push(idk::Pointlight());
            return m_pointlights.size() - 1;

        case lightsource::SPOT:
            m_spotlights.push(idk::Spotlight());
            return m_spotlights.size() - 1;

        case lightsource::DIR:
            const int id = m_dirlights.size();

            m_dirlights.push_back(idk::Dirlight());
            m_dirlight_shadowmaps.push_back(glFramebuffer());
            m_dirlight_shadowmaps[id].reset(2048, 2048, 1);
            m_dirlight_shadowmaps[id].colorAttachment(0, m_dirlight_config);

            return id;
    }

    return -1;
}


void
idk::LightSystem::destroyLightsource( idk::lightsource type, int id )
{
    m_changed = true;

    switch (type)
    {
        case lightsource::POINT:
        break;

        case lightsource::SPOT:     break;

        case lightsource::DIR:
            while (m_dirlights.size() > 0)
            {
                if (m_dirlights.size() - 1 != id)
                {
                    m_dirlights_temp.push_back(m_dirlights.back());
                    m_dirlights.pop_back();
                }

                else
                {
                   m_dirlights.pop_back();
                }
            }

            idk::swap(m_dirlights, m_dirlights_temp);
        break;
    }

}



bool
idk::LightSystem::changed()
{
    bool ch = m_changed;
    m_changed = false;

    return ch;
};



void
idk::LightSystem::update()
{

}


