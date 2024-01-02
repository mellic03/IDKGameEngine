#include "idk_batch_allocator.hpp"

#include <libidk/idk_assert.hpp>
#include "../../model/idk_model.hpp"

static constexpr size_t INITIAL_ALLOC_SIZE = 128 * idk::MEGA;


idk::BatchAllocator::BatchAllocator( size_t nbytes, uint32_t vertexformat )
{
    auto desc = idk::getVertexFormatDescriptor(vertexformat);

    GLuint     bindingindex = 0;
    GLbitfield buffer_flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT;

    gl::createBuffers(1, &VBO);
    gl::namedBufferStorage(VBO, nbytes, nullptr, buffer_flags);
    VBO_ptr = gl::mapNamedBuffer(VBO, GL_WRITE_ONLY);
    IDK_ASSERT("Error mapping idk::internal::Batch::VBO", VBO_ptr != nullptr);

    gl::createBuffers(1, &IBO);
    gl::namedBufferStorage(IBO, nbytes, nullptr, buffer_flags);
    IBO_ptr = gl::mapNamedBuffer(IBO, GL_WRITE_ONLY);
    IDK_ASSERT("Error mapping idk::internal::Batch::IBO", IBO_ptr != nullptr);

    gl::createVertexArrays(1, &VAO);
    gl::vertexArrayVertexBuffer(VAO, bindingindex, VBO, 0, desc.stride);
    gl::vertexArrayElementBuffer(VAO, IBO);

    for (int i=0; i<desc.attribs; i++)
    {
        gl::enableVertexArrayAttrib(VAO, i);
        gl::vertexArrayAttribBinding(VAO, i, bindingindex);
    
        gl::vertexArrayAttribFormat(
            VAO,
            i,
            desc.attrib_size[i],
            desc.attrib_datatype[i],
            GL_FALSE,
            desc.attrib_offset[i]
        );
    }
}


idk::ModelHandle
idk::BatchAllocator::load( size_t v_nbytes, float *vertices,
                                     size_t i_nbytes, uint32_t *indices )
{
    idk::ModelHandle handle = {
        .vertexformat  = 0,
        .batch_id      = 0,
        .base_vertex   = 0,
        .base_instance = 0
    };

    return handle;
}



void *
idk::BatchAllocator::reserveVertices( size_t nbytes )
{
    VBO_cursor += nbytes;
    return (uint8_t *)VBO_ptr + VBO_cursor;
}


void *
idk::BatchAllocator::reserveIndices( size_t nbytes )
{
    IBO_cursor += nbytes;
    return (uint8_t *)IBO_ptr + IBO_cursor;
}


