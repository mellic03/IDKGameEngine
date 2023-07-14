#pragma once


#include <vector>
#include <string>
#include "idk_module.h"

namespace idk { class Engine; };
namespace idk { class ComponentSystem; };


/**
 * ## Dependency-related methods are only called if IDK_DEBUG is defined
*/

class idk::ComponentSystem: public idk::Module
{
public:
    virtual         ~ComponentSystem() = default;

    /*              Called when the component is assigned. */
    virtual void    onAssignment( int obj_id, idk::Engine & ) = 0;

    /*              Called when a new idk::GameObject is created. */
    virtual void    onGameObjectCreation( int obj_id, idk::Engine & ) = 0;

    /*              Called when an idk::GameObject is deleted. */
    virtual void    onGameObjectDeletion( int obj_id, idk::Engine & ) = 0;

    /*              Called when an idk::GameObject is created by copy. */
    virtual void    onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & ) = 0;

};


