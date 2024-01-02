#pragma once

#include <vector>
#include <string>

namespace idk { class Engine; };
namespace idk { class RuntimeModule; };
namespace idk { class TestV; };


class idk::RuntimeModule
{
protected:
    std::string                         m_name;
    std::vector<std::string>            m_dependencies;

public:
    virtual                            ~RuntimeModule() = default;

    const constexpr std::string &       name() const { return m_name; };

    virtual void                        setup   ( idk::Engine & ) = 0;
    virtual void                        stage_A ( idk::Engine & ) = 0;
    virtual void                        stage_B ( idk::Engine & ) = 0;
    virtual void                        stage_C ( idk::Engine & ) = 0;

};

