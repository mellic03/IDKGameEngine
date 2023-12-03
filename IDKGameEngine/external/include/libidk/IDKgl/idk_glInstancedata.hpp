#pragma once

#include "common.hpp"
#include "idk_glBindings.hpp"


namespace idk { class glInstancedTransforms; };


class idk::glInstancedTransforms
{
private:
    GLuint m_VBO;

public:
    std::vector<glm::mat4> data;

    void         init( size_t num_elements );
    void         resize( size_t num_elements );
    glm::mat4 &  get( int idx );
    void         update();
    void         bind();
    void         unbind();

};
