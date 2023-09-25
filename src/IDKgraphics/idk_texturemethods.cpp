#include "IDKcommon/IDKcommon.h"

#include "IDKglInterface/IDKglInterface.h"


using namespace idk;


/** Run a shader with two input framebuffers, `a` and `b`.
 *  Render the result to `out`.
 * 
 *  @note `a`, `b` and `out` cannot have more than four textures each.
 * 
*/
void
tex2tex( GLuint program, glFramebuffer &a, glFramebuffer &b, glFramebuffer &out )
{
    gltools::bindIdkFramebuffer(out);
    gltools::useProgram(program);

    size_t textureID = 0;
    for (size_t i=0; i < a.output_textures.size(); i++)
    {
        gltools::setUniform_texture(
            "un_texture_" + std::to_string(textureID),
            a.output_textures[i]
        );

        textureID += 1;
    }

    textureID = 4;
    for (size_t i=0; i < b.output_textures.size(); i++)
    {
        gltools::setUniform_texture(
            "un_texture_" + std::to_string(textureID),
            b.output_textures[i]
        );

        textureID += 1;
    }

    // gl::bindVertexArray(m_quad_VAO);
    // gl::drawArrays(GL_TRIANGLES, 0, 6);

    gltools::freeTextureUnitIDs();
};


