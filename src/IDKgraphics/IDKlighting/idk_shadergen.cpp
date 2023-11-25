#include "idk_shadergen.hpp"
#include "idk_shader_components.hpp"
#include "idk_lightsource.hpp"


static const std::string &
beginHeader()
{
    return idk_shadergen::shader_begin_header;
}


static const std::string &
endHeader()
{
    return idk_shadergen::shader_end_header;
}


static const std::string &
beginMain()
{
    return idk_shadergen::shader_begin_main;
}


static const std::string &
endMain()
{
    return idk_shadergen::shader_end_main;
}


static const std::string
genLightCall( const idk::vector<uint32_t> &flags, 
              const std::string &call_none, const std::string &call_shadow )
{
    static std::string source = "";
    source.clear();

    for (size_t i=0; i<flags.size(); i++)
    {
        if (flags[i] == idk::LightFlag::NONE)
        {
            source += call_none;
        }

        else if (flags[i] == idk::LightFlag::SHADOWMAP)
        {
            source += call_shadow;
        }

        source += std::to_string(i);
        source += idk_shadergen::light_call_end;
    }

    return source;
}



static void
genVertShader( std::string &vert )
{
    vert += idk_shadergen::shader_screenquad;
}


static void
genFragShader( const idk::shadergen::Config &config, std::string &frag )
{
    frag += beginHeader();
    {
        frag += idk_shadergen::pointlight_definition;
        // frag += idk_shadergen::dirlight_definition;
    }
    frag += endHeader();
    
    frag += idk_shadergen::pointlight_contribution;
    // frag += idk_shadergen::dirlight_contribution;
    // frag += idk_shadergen::dirlight_contribution_shadowmapped;

    frag += beginMain();
    {
        frag += genLightCall(
            config.point_flags,
            idk_shadergen::pointlight_call_begin,
            idk_shadergen::pointlight_shadowmapped_call_begin
        );

        // frag += genLightCall(
        //     config.dir_flags,
        //     idk_shadergen::dirlight_call_begin,
        //     idk_shadergen::dirlight_shadowmapped_call_begin
        // );
    }
    frag += endMain();
}


void
idk::shadergen::genShaderString( const idk::shadergen::Config &config,
                                 std::string &vert, std::string &frag )
{
    vert.clear();
    frag.clear();

    genVertShader(vert);
    genFragShader(config, frag);
}

