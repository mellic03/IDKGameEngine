#pragma once

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKRenderEngine/IDKRenderEngine.hpp>


namespace idk { class Name_CS; };

class idk::Name_CS: public idk::ComponentSystem
{
private:
    idk::Allocator<std::string> m_names;
    std::map<int, int>          m_keys;

public:

    void        init    ( idk::EngineAPI & );
    void        stage_A ( idk::EngineAPI & ) {  };
    void        stage_B ( idk::EngineAPI & ) {  };
    void        stage_C ( idk::EngineAPI & ) {  };

    void        onObjectAssignment ( int obj_id, idk::Engine & );
    void        onObjectCreation   ( int obj_id, idk::Engine & );
    void        onObjectDeletion   ( int obj_id, idk::Engine & );
    void        onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );

    void        setName( int obj_id, const std::string &name );
    std::string &getName( int obj_id );

};


