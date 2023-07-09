#pragma once


#include <vector>
#include <string>


namespace idk { class Engine; };
namespace idk { class ComponentSystem; };


/**
 * ## Dependency-related methods are only called if IDK_DEBUG is defined
*/

class idk::ComponentSystem
{
protected:
    int                         _component_index;
    std::string                 _name;
    std::vector<std::string>    _dependencies;
    std::vector<std::string>    _incompatibilities;

public:
    virtual         ~ComponentSystem() = default;

    void    addDependency( std::string name ) { _dependencies.push_back(name); };
    void    addDependencies()                 { /* Base case */                };

    template <typename ...Args> void    
            addDependencies( std::string head, Args... );

    const std::vector<std::string> &
            getDependencies() const { return _dependencies; };

    // void    addIncompatibility( std::string name ) { _incompatibilities.push_back(name); };
    // void    addIncompatibilities()                 { /* Base case */                     };
    // template <typename ...Args> void
    //         addIncompatibilities( std::string head, Args... );
    // const std::vector<std::string> &
    //         getIncompatibilities() const { return _incompatibilities; };

    void            base_init( int id, std::string name ) { _component_index = id; _name = name; };
    std::string     name() const { return _name; };

    virtual void    init( idk::Engine & ) = 0;
    virtual void    stage_A( idk::Engine & ) = 0;
    virtual void    stage_B( idk::Engine & ) = 0;


    void    addDependency( std::string name ) { _dependencies.push_back(name); };
    void    addDependencies()                 { /* Base case */                };

    template <typename ...Args> void    addDependencies( std::string head, Args... );
    const std::vector<std::string> &    getDependencies() const { return _dependencies; };


    /*              Called when the component is assigned.
    */
    virtual void    onAssignment( int obj_id, idk::Engine & ) = 0;

    /*              Called when a new idk::GameObject is created. */
    virtual void    onGameObjectCreation( int obj_id, idk::Engine & ) = 0;

    /*              Called when an idk::GameObject is deleted. */
    virtual void    onGameObjectDeletion( int obj_id, idk::Engine & ) = 0;

    /*              Called when an idk::GameObject is created by copy. */
    virtual void    onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & ) = 0;

};



template <typename ...Args>
void
idk::ComponentSystem::addDependencies( std::string head, Args... rest )
{
    addDependency(head);
    addDependencies(rest...);
}




// template <typename ...Args>
// void
// idk::ComponentSystem::addIncompatibilities( std::string head, Args... rest )
// {
//     addIncompatibility(head);
//     addIncompatibilities(rest...);
// }



// /*              ----- Only called if IDK_DEBUG is defined -----

//                 Ensure an object has all required components.
//                 E.g. model component may need a transform component.
//                 Called when the component is assigned but before onAssignment().
// */
// void            checkDependencies( int obj_id, idk::Engine & );
