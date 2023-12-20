#include "idk_renderqueue.hpp"


void
idk::RenderQueue::setViewParams( float cam_near, float cam_far, const glm::mat4 &V )
{
    m_cam_near = cam_near;
    m_cam_far  = cam_far;
    m_cam_view = V;
}



void
idk::RenderQueue::push( int model_id, int animator_id, const glm::mat4 &transform )
{
    /*
        The view frustum is split into NUM_CASCADES cascades.
        This is to draw the scene front to back, reducing overdraw.
    */

    static glm::vec4 worldspace = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    static glm::vec4 viewspace;

    viewspace = m_cam_view * transform * worldspace;

    float  depth   = fabs(viewspace.z);
    size_t cascade = size_t(NUM_CASCADES * (depth / m_cam_far));

    if (cascade >= NUM_CASCADES)
    {
        return; // Beyond far plane
    }

    m_queue[cascade].push_back({model_id, animator_id, transform});
}


void
idk::RenderQueue::push( int model_id, const glm::mat4 &transform )
{
    push(model_id, -1, transform);
}



void
idk::RenderQueue::clear()
{
    for (size_t i=0; i<NUM_CASCADES; i++)
    {
        m_queue[i].clear();
    }
}




idk::RenderQueue::iterator::iterator( RenderQueue *rq )
{
    m_RQ = rq;

    if (rq == nullptr)
    {
        cascade = -1;
        idx     = -1;
    }
    
    else
    {
        cascade = 0;
        idx     = -1;

        increment(*this);
    }
}


idk::RenderQueue::iterator::iterator( const iterator &other )
{
    this->m_RQ    = other.m_RQ;
    this->cascade = other.cascade;
    this->idx     = other.idx;
}


void
idk::RenderQueue::iterator::increment( iterator &it )
{
    it.idx += 1;

    while (it.idx >= it.m_RQ->m_queue[it.cascade].size())
    {
        it.idx = 0;
        it.cascade += 1;

        if (it.cascade >= NUM_CASCADES)
        {
            it = iterator(nullptr);
            break;
        }
    }
}


