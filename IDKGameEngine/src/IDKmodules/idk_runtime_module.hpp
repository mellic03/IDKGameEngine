#pragma once

#include <vector>
#include <string>

namespace idk { class Engine; };
namespace idk { class RuntimeModule; };
namespace idk { class TestV; };


class idk::RuntimeModule
{
protected:
    int                                 m_id;
    std::string                         m_name;
    std::vector<std::string>            m_dependencies;

public:
    virtual                            ~RuntimeModule() = default;

    void                                init( int id ) { m_id = id; };
    
    const constexpr int                 ID() const { return m_id; };
    const constexpr std::string &       name() const { return m_name; };


    virtual void                        setup   ( idk::Engine & ) {  };
    virtual void                        stage_A ( idk::Engine & ) {  };
    virtual void                        stage_B ( idk::Engine & ) {  };
    virtual void                        stage_C ( idk::Engine & ) {  };

};

