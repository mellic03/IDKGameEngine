#pragma once

#include <string>


namespace idk { class Game; class GameHandle; };
namespace idk { class Engine; class RenderEngine; };


class idk::Game
{
private:
    int         m_id   = -1;
    std::string m_name = "WOOP";

public:
                    Game( std::string name ): m_name(name) {  };

    int             ID()   const { return m_id; };
    std::string     name() const { return m_name; };

    virtual void    config() = 0;

    virtual void    registerModules ( idk::Engine &) = 0;

    virtual void    setup    ( idk::Engine &, idk::RenderEngine & ) = 0;
    virtual void    mainloop ( idk::Engine &, idk::RenderEngine & ) = 0;

};


class idk::GameHandle
{
private:
    typedef idk::Game *(*getInstance_handle)();

    std::string  m_filepath;
    void        *m_handle;
    void        *m_function;

public:

    GameHandle( std::string relpath );
    ~GameHandle();

    idk::Game *getInstance();

};



