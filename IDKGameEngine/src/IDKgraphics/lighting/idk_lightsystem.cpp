#include "idk_lightsystem.hpp"


void
idk::LightSystem::init()
{
    m_framebuffer_config = {
        .internalformat = GL_RGBA16F,
        .minfilter      = GL_NEAREST,
        .magfilter      = GL_NEAREST,
        .datatype       = GL_FLOAT
    };


    constexpr int size = 10;

    m_shadergen_config.point_flags.reserve(size);
    m_shadergen_config.spot_flags.reserve(size);
    m_shadergen_config.dir_flags.reserve(size);

    m_point_vec.reserve(size);
    m_spot_vec.reserve(size);
    m_dir_vec.reserve(size);

    m_depthcascade.reset(2048, 2048);
    m_depthcascade.setCascadeDepths( 0.1f, 0.2f, 0.5f, 1.0f );

}


int
idk::LightSystem::createPointlight( uint32_t flags )
{
    return createLightsource(
        m_point_alc, m_point_vec,
        m_shadergen_config.point_flags,
        flags
    );
}


int
idk::LightSystem::createDirlight( uint32_t flags )
{
    const int id = createLightsource(
        m_dir_alc, m_dir_vec,
        m_shadergen_config.dir_flags,
        flags
    );

    return id;
}


void
idk::LightSystem::destroyPointlight( int id )
{
    destroyLightsource(
        m_point_alc, m_point_vec,
        m_shadergen_config.point_flags,
        id
    );
}


void
idk::LightSystem::destroyDirlight( int id )
{
    destroyLightsource(
        m_dir_alc, m_dir_vec,
        m_shadergen_config.dir_flags,
        id
    );
}


bool
idk::LightSystem::changed()
{
    bool ch = m_changed;
    m_changed = false;

    return ch;
};


void
idk::LightSystem::genShaderString( std::string &vert, std::string &frag )
{
    shadergen::genShaderString(m_shadergen_config, vert, frag);
}

