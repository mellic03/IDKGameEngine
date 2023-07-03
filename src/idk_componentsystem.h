#pragma once


namespace idk { class Engine; };
namespace idk { class ComponentSystem; };

class idk::ComponentSystem
{
protected:
    int         _component_index = -1;
    std::string _name;

public:
    virtual         ~ComponentSystem() = default;

    virtual void    base_init( int id, std::string name ) { _component_index = id; _name = name; };
    std::string     name() const { return _name; };

    virtual void    init( idk::Engine & ) = 0;
    virtual void    stage_A( idk::Engine & ) = 0;
    virtual void    stage_B( idk::Engine & ) = 0;

    
    /* Called when the component is assigned */
    virtual void    onAssignment( int obj_id, idk::Engine & ) {  };

    /* Called when a new idk::GameObject is created */
    virtual void    onGameObjectCreation( int obj_id ) {  };
    
    /* Called when an idk::GameObject is deleted */
    virtual void    onGameObjectDeletion( int obj_id ) {  };
    
    /* Called when an idk::GameObject is created by copy */
    virtual void    onGameObjectCopy( int src_obj_id, int dest_obj_id ) {  };

};

