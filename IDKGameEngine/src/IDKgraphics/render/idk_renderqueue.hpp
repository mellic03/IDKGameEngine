#pragma once

#include <libidk/IDKcore/libidk.hpp>
#include "../model/idk_OBB.hpp"
#include <tuple>


namespace idk { struct RenderQueueConfig; class RenderQueue; };


struct idk::RenderQueueConfig
{
    bool cull_face = true;
};


class idk::RenderQueue
{
private:
    static const size_t NUM_CASCADES = 10;

    std::string         m_name;
    float               m_cam_near;
    float               m_cam_far;
    glm::mat4           m_cam_view;
    glm::mat4           m_cam_proj;
    RenderQueueConfig   m_config;

    using subqueue_t = std::vector<std::tuple<int, int, glm::mat4>>;
    std::vector<subqueue_t> m_queue;


public:
            RenderQueue(): m_name("None"), m_queue(NUM_CASCADES) {  };
            RenderQueue( const std::string &name, const RenderQueueConfig & );

    constexpr const std::string         &name()   const { return m_name;   };
    constexpr const RenderQueueConfig   &config() const { return m_config; };

    void    setViewParams( float cam_near, float cam_far, const glm::mat4 &P, const glm::mat4 &V );
    void    push( int model_id, int animator_id, const glm::mat4 &transform );
    void    push( int model_id, const glm::mat4 &transform );
    void    clear();


    struct iterator
    {
        RenderQueue *m_RQ    = nullptr;
        int          cascade = 0;
        int          idx     = 0;

                    iterator( RenderQueue *rq );
                    iterator( const iterator &other );
        void        increment( iterator &it );

        iterator &operator ++ ()
        {
            increment(*this);
            return *this;
        }

        iterator operator ++ (int)
        {
            iterator temp = *this;
            increment(temp);
            return temp;
        }

        bool operator == ( const iterator &rhs )
        {
            return m_RQ == rhs.m_RQ && cascade == rhs.cascade && idx == rhs.idx;
        }
    
        bool operator != ( const iterator &rhs )
        {
            return m_RQ != rhs.m_RQ && cascade != rhs.cascade && idx != rhs.idx;
        }

        std::tuple<int, int, glm::mat4> &operator * ()
        {
            return m_RQ->m_queue[cascade][idx];
        }
    };

    iterator begin() { return iterator(this);    }
    iterator end()   { return iterator(nullptr); }

};

