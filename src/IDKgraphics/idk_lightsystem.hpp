#pragma once

#include "IDKgl/IDKgl.hpp"
#include "IDKlightsource/IDKlightsource.hpp"

namespace idk { class LightSystem; };


class idk::LightSystem
{
private:

    idk::vector<Pointlight>  m_pointlights;
    idk::vector<Spotlight>   m_spotlights;

    idk::vector<Dirlight>           m_dirlights;
    idk::vector<glFramebuffer>      m_dirlight_shadowmaps;
    ColorAttachmentConfig           m_dirlight_config;


    idk::vector<Pointlight>     m_pointlights_temp;
    idk::vector<Spotlight>      m_spotlights_temp;
    idk::vector<Dirlight>       m_dirlights_temp;

    bool m_changed = true;


public:

    void init();

    Pointlight &    getPointlight ( int id ) { return this->m_pointlights[id];  };
    Spotlight  &    getSpotlight  ( int id ) { return this->m_spotlights[id];   };
    Dirlight   &    getDirlight   ( int id ) { return this->m_dirlights[id];    };


    int             createLightsource  ( idk::lightsource type         );
    void            destroyLightsource ( idk::lightsource type, int id );


    idk::vector<Pointlight> &       pointlights () { return m_pointlights;         };
    idk::vector<Dirlight>   &       dirlights   () { return m_dirlights;           };
    idk::vector<glFramebuffer>  &   shadowmaps  () { return m_dirlight_shadowmaps; };


    /** True if the number of lightsources have changed since the last call to changed(). */
    bool changed();

    void update();

};

