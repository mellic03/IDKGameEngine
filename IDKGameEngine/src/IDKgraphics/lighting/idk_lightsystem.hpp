#pragma once

#include "libidk/libidk.hpp"
#include "libidk/IDKgl.hpp"
#include "idk_lightsource.hpp"
#include "idk_shadergen.hpp"
#include "idk_shadowcascade.hpp"

namespace idk { class LightSystem; };


class idk::LightSystem
{
private:

    idk::Allocator<Pointlight>  m_point_alc;
    idk::Allocator<Spotlight>   m_spot_alc;
    idk::Allocator<Dirlight>    m_dir_alc;

    std::vector<Pointlight>     m_point_vec;
    std::vector<Spotlight>      m_spot_vec;
    std::vector<Dirlight>       m_dir_vec;

    idk::glDepthCascade         m_depthcascade;
    // std::vector<glFramebuffer>  m_dirlight_shadowmaps;
    glTextureConfig               m_framebuffer_config;
    DepthAttachmentConfig       m_depthbuffer_config;

    idk::shadergen::Config      m_shadergen_config;
    std::vector<uint32_t>       m_temp_flags;

    bool m_changed = true;

    template <typename light_t>
    int             createLightsource( idk::Allocator<light_t> &a_lights,
                                       std::vector<light_t> &v_lights,
                                       std::vector<uint32_t> &config_flags,
                                       uint32_t flags );

    template <typename light_t>
    void            destroyLightsource( idk::Allocator<light_t> &a_lights,
                                        std::vector<light_t> &v_lights,
                                        std::vector<uint32_t> &flags,
                                        int id );

public:

    void            init();

    Pointlight &    getPointlight     ( int id ) { return this->m_point_vec[id]; };
    Spotlight  &    getSpotlight      ( int id ) { return this->m_spot_vec[id];  };
    Dirlight   &    getDirlight       ( int id ) { return this->m_dir_vec[id];   };

    int             createPointlight  ( uint32_t flags=idk::LightFlag::NONE );
    int             createDirlight    ( uint32_t flags=idk::LightFlag::NONE );
    void            destroyPointlight ( int id );
    void            destroyDirlight   ( int id );

    std::vector<Pointlight> &       pointlights () { return m_point_vec;       };
    std::vector<Dirlight>   &       dirlights   () { return m_dir_vec;         };
    // std::vector<glFramebuffer> &    shadowmaps  () { return m_dirlight_shadowmaps; };
    idk::glDepthCascade &           depthCascade() { return m_depthcascade; };

    /** True if the number of lightsources have changed since the last call to changed(). */
    bool changed();

    void genShaderString( std::string &vert, std::string &frag );
};



template <typename light_t>
int
idk::LightSystem::createLightsource( idk::Allocator<light_t> &a_lights,
                                     std::vector<light_t> &v_lights,
                                     std::vector<uint32_t> &config_flags,
                                     uint32_t flags )
{
    m_changed = true;

    const int id = a_lights.create();
    v_lights = a_lights.vector();

    config_flags.resize(v_lights.size());
    config_flags[id] = flags;

    return id;
}



template <typename light_t>
void
idk::LightSystem::destroyLightsource( idk::Allocator<light_t> &a_lights,
                                      std::vector<light_t> &v_lights,
                                      std::vector<uint32_t> &config_flags,
                                      int id )
{
    m_changed = true;

    a_lights.destroy(id);
    v_lights = a_lights.vector();

    config_flags.resize(v_lights.size());
    config_flags[id] = idk::LightFlag::NONE;
}

