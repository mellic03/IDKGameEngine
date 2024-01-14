#pragma once

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKGraphics/IDKGraphics.hpp>
#include "../../external/include/idk_icons/idk_Icons.hpp"


namespace idk { class Icon_CS; };

class idk::Icon_CS: public idk::ComponentSystem
{
private:
    std::string m_default_icon = IDK_ICON_BOX;

    idk::Allocator<std::string>    m_icons;
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

    void        onObjectSelection( int obj_id );


    void        setDefaultIcon( const std::string &icon ) { m_default_icon = icon; };

    void         setIcon( int obj_id, const std::string &icon );
    std::string &getIcon( int obj_id );

};


