#pragma once

#include <libidk/idk_allocator.hpp>
#include <libidk/GL/common.hpp>
#include <libidk/GL/idk_glXXBO.hpp>

#include <libidk/idk_singleton.hpp>

#include "../../model/idk_model.hpp"
#include "idk_model_handle.hpp"
#include "idk_batch_allocator.hpp"



namespace idk { class BatchSystem; };


class idk::BatchSystem: public idk::basic_singleton<idk::BatchSystem>
{
private:
    friend class idk::basic_singleton<idk::BatchSystem>;
    BatchSystem();

    static constexpr size_t     BATCH_SIZE = 128 * idk::MEGA;

    std::vector<BatchAllocator> m_batches;
    Allocator<ModelHandle>      m_handles;


public:

    int load( uint32_t vertexformat, size_t v_nbytes, float *vertices,
              size_t i_nbytes, uint32_t *indices );

    const idk::ModelHandle &getModelHandle( int id );
};

// namespace idk::BatchSystem
// {
//     size_t init( size_t bytes_per_batch );

//     int load( uint32_t vertexformat, size_t v_nbytes, float *vertices,
//               size_t i_nbytes, uint32_t *indices );

//     const idk::ModelHandle &getModelHandle( int id );

// };

