#include <libidk/idk_assert.hpp>
#include <IDKGameEngine/IDKengine.hpp>
#include <IDKGameEngine/IDKmodules/idk_game.hpp>

#include <string>


class GameTemplate: public idk::Game
{
private:


public:

    GameTemplate( std::string name ): Game(name) {  };

    virtual void config() override;
    virtual void registerModules ( idk::Engine & ) override;
    virtual void setup( idk::Engine &, idk::RenderEngine & ) override;
    virtual void mainloop( idk::Engine &, idk::RenderEngine & ) override;

};

