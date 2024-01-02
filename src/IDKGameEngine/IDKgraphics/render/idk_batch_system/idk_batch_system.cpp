#include "idk_batch_system.hpp"


idk::BatchSystem::BatchSystem()
{
    for (uint32_t i=0; i<idk::VERTEX_NUM_FORMATS; i++)
    {
        m_batches[i] = BatchAllocator(BATCH_SIZE, i);
    }
}


int
idk::BatchSystem::load( uint32_t vertexformat, size_t v_nbytes, float *vertices,
                        size_t i_nbytes, uint32_t *indices )
{
    auto &batch = m_batches[vertexformat];

    // void *data = batch.reserve(v_nbytes);

    auto handle = batch.load(v_nbytes, vertices, i_nbytes, indices);

    return m_handles.create(handle);
}


const idk::ModelHandle &
idk::BatchSystem::getModelHandle( int id )
{
    return m_handles.get(id);
}

