#pragma once

#include "idk_model_handle.hpp"


namespace idk
{
    static constexpr size_t KILO = 1024;
    static constexpr size_t MEGA = 1024*1024;

    class BatchAllocator;
};


class idk::BatchAllocator
{
private:
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;

    void  *VBO_ptr;
    void  *IBO_ptr;

    size_t VBO_cursor = 0;
    size_t IBO_cursor = 0;

public:
            BatchAllocator() {  };
            BatchAllocator( size_t nbytes, uint32_t vertexformat );

    idk::ModelHandle    load( size_t v_nbytes, float *vertices,
                              size_t i_nbytes, uint32_t *indices );

    void *              reserveVertices( size_t nbytes );
    void *              reserveIndices( size_t nbytes );

};