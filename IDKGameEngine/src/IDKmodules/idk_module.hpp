#pragma once

#include <vector>
#include <string>

namespace idk { class EngineAPI; };
namespace idk { class Module; };


class idk::Module
{
protected:
    int                                 m_id;
    std::string                         m_name;
    std::vector<std::string>            m_dependencies;

public:
    virtual                             ~Module() = default;
    void                                base_init( int id, std::string name ) { m_id = id; m_name = name; };
    
    const constexpr int                 ID() const { return m_id; };
    const constexpr std::string &       name() const { return m_name; };

    virtual void                        init( idk::EngineAPI & ) = 0;
    virtual void                        stage_A( idk::EngineAPI & ) = 0;
    virtual void                        stage_B( idk::EngineAPI & ) = 0;
    virtual void                        stage_C( idk::EngineAPI & ) = 0;

    void                                addDependency( std::string name ) { m_dependencies.push_back(name); };
    void                                addDependencies()                 { /* Base case */ };
    template <typename ...Args> void    addDependencies( std::string head, Args... );
    const std::vector<std::string> &    getDependencies() const { return m_dependencies; };

};



template <typename ...Args>
void
idk::Module::addDependencies( std::string head, Args... rest )
{
    addDependency(head);
    addDependencies(rest...);
}

