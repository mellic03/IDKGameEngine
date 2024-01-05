#pragma once

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKRenderEngine/IDKRenderEngine.hpp>


namespace idk { class Model_CS; };


class idk::Model_CS: public idk::ComponentSystem
{
private:
    idk::Allocator<std::tuple<int, int, int>>   m_object_model_ids;
    std::map<int, int>      m_keys;
    std::map<int, bool>     m_shadowcasts;

public:
    void            init( idk::EngineAPI & );
    void            stage_A( idk::EngineAPI & );
    void            stage_B( idk::EngineAPI & ) {  };
    void            stage_C( idk::EngineAPI & ) {  };

    void            onObjectAssignment( int obj_id, idk::Engine & ) {  };
    void            onObjectCreation( int obj_id, idk::Engine & )   {  };
    void            onObjectDeletion( int obj_id, idk::Engine & );
    void            onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );

    void            useModel( idk::RenderEngine &, int obj_id, int model_id );
    idk::Model &    getModel( idk::RenderEngine &, int obj_id );
    int             getModelID( int obj_id );

    idk::Animator & getAnimator( idk::RenderEngine &, int obj_id);
    int             getAnimatorID( idk::RenderEngine &, int obj_id );

    void            setShadowcast( int obj_id, bool b ) { m_shadowcasts[obj_id] = b; };

};

