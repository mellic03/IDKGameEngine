#pragma once

#include <vector>
#include <string>

namespace idk { class Engine; };
namespace idk { class Module; };


class idk::Module
{
protected:
    int                         _component_index;
    std::string                 _name;
    std::vector<std::string>    _dependencies;

public:
    virtual                     ~Module() = default;
    void                        base_init( int id, std::string name ) { _component_index = id; _name = name; };
    std::string                 name() const { return _name; };

    virtual void                init( idk::Engine & ) = 0;
    virtual void                stage_A( idk::Engine & ) = 0;
    virtual void                stage_B( idk::Engine & ) = 0;

    void                                addDependency( std::string name ) { _dependencies.push_back(name); };
    void                                addDependencies()                 { /* Base case */                };
    template <typename ...Args> void    addDependencies( std::string head, Args... );
    const std::vector<std::string> &    getDependencies() const { return _dependencies; };

};



template <typename ...Args>
void
idk::Module::addDependencies( std::string head, Args... rest )
{
    addDependency(head);
    addDependencies(rest...);
}

