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
            return  m_pointlights.size() - 1;

        case lightsource::SPOT:
            m_spotlights.push(idk::Spotlight());
            return m_spotlights.size() - 1;

        case lightsource::DIR:
            const int id = m_dirlights.size();
            m_dirlights.push(idk::Dirlight());
            m_dirlight_shadowmaps.push(glFramebuffer());
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
            while (m_pointlights.size() > 0)
            {
                if (m_pointlights.size() - 1 != id)
                {
                    m_pointlights_temp.push(m_pointlights.pop());
                }
                else
                {
                   m_pointlights.pop();
                }
            }

            idk::swap(m_pointlights, m_pointlights_temp);
        break;

        case lightsource::SPOT:     break;

        case lightsource::DIR:
            while (m_dirlights.size() > 0)
            {
                if (m_dirlights.size() - 1 != id)
                {
                    m_dirlights_temp.push(m_dirlights.pop());
                }
                else
                {
                   m_dirlights.pop();
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


