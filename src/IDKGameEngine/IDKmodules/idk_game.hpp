#pragma once

#include <libidk/idk_export.hpp>
#include <string>


namespace idk { class Game; class GameHandle; };
namespace idk { class EngineAPI; };


class IDK_VISIBLE idk::Game
{
private:
    int         m_id   = -1;
    std::string m_name = "WOOP";

public:
                    Game( std::string name ): m_name(name) {  };

    int             ID()   const { return m_id; };
    std::string     name() const { return m_name; };

    virtual void    config() = 0;

    virtual void    registerModules ( idk::EngineAPI & ) = 0;

    virtual void    setup    ( idk::EngineAPI & ) = 0;
    virtual void    mainloop ( idk::EngineAPI & ) = 0;

};
