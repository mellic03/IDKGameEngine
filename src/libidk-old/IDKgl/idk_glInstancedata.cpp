#include "idk_glInstancedata.hpp"


void
idk::glInstancedTransforms::init( size_t num_elements )
{
    data.resize(num_elements, glm::mat4(1.0f));

    gl::genBuffers(1, &m_VBO);
    gl::bindBuffer(GL_ARRAY_BUFFER, m_VBO);
    gl::bufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*num_elements, &data[0], GL_STATIC_DRAW);

    constexpr size_t start_attr = 4;

    gl::vertexAttribPointer(start_attr+0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 0*sizeof(glm::vec4));
    gl::enableVertexAttribArray(start_attr+0); 

    gl::vertexAttribPointer(start_attr+1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 1*sizeof(glm::vec4));
    gl::enableVertexAttribArray(start_attr+1); 

    gl::vertexAttribPointer(start_attr+2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 2*sizeof(glm::vec4));
    gl::enableVertexAttribArray(start_attr+2); 

    gl::vertexAttribPointer(start_attr+3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 3*sizeof(glm::vec4));
    gl::enableVertexAttribArray(start_attr+3);

    GLCALL( glVertexAttribDivisor(start_attr+0, 1); )
    GLCALL( glVertexAttribDivisor(start_attr+1, 1); )
    GLCALL( glVertexAttribDivisor(start_attr+2, 1); )
    GLCALL( glVertexAttribDivisor(start_attr+3, 1); )

    gl::bindBuffer(GL_ARRAY_BUFFER, 0);
 
}


void
idk::glInstancedTransforms::resize( size_t num_elements )
{
    data.resize(num_elements, glm::mat4(1.0f));

    gl::bindBuffer(GL_ARRAY_BUFFER, m_VBO);
    gl::bufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*num_elements, &data[0], GL_STATIC_DRAW);

    gl::bindBuffer(GL_ARRAY_BUFFER, 0);
}


void
idk::glInstancedTransforms::update()
{
    gl::bindBuffer(GL_ARRAY_BUFFER, m_VBO);
    gl::bufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*data.size(), &data[0], GL_STATIC_DRAW);
    gl::bindBuffer(GL_ARRAY_BUFFER, 0);
}


glm::mat4 &
idk::glInstancedTransforms::get( int idx )
{
    if (idx > data.size())
    {
        data.resize(idx+1);
    }

    return data[idx];
}


void
idk::glInstancedTransforms::bind()
{
    gl::bindBuffer(GL_ARRAY_BUFFER, m_VBO);
}


void
idk::glInstancedTransforms::unbind()
{
    gl::bindBuffer(GL_ARRAY_BUFFER, 0);
}

