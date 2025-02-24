#pragma once

#include <libidk/idk_export.hpp>
#include <libidk/idk_window.hpp>
#include <libidk/idk_module.hpp>
#include <libidk/idk_dynamiclib.hpp>
#include <libidk/idk_game.hpp>
#include <libidk/memory/linear_allocator.hpp>
#include <libidk/memory/stack_allocator.hpp>
#include <libidk/memory/ring_allocator.hpp>

#include <string>
#include <vector>
#include <map>
#include <functional>



namespace idk
{
    class Window;
    class GLContext;

    class linear_allocator;

    class Engine;

    template <typename key_type, typename msg_type>
    class EventEmitter;

    class ECS;
    class IO;
    class Audio;

    class RenderEngine;
    class Game;

    class EngineAPI;


    struct EngineMemory
    {
        static constexpr size_t MAIN_SIZE  = 1024 * idk::MEGA;
        static constexpr size_t STACK_SIZE = 16   * idk::MEGA;
        static constexpr size_t RING_SIZE  = 16   * idk::MEGA;

        idk::linear_allocator *mainblock;
        idk::stack_allocator  *stack;
        idk::ring_allocator   *ring;

        EngineMemory()
        {
            using namespace idk;

            mainblock = new linear_allocator(MAIN_SIZE);
            stack     = new stack_allocator(STACK_SIZE, mainblock);
            ring      = new ring_allocator(RING_SIZE, mainblock);
        }
    };

}



class IDK_VISIBLE idk::EngineAPI
{
private:
    std::string         m_name;
    float               m_dt = 0.001f;

    std::vector<std::string> m_args;
    std::vector<std::function<void()>> m_callbacks;
    std::map<std::string, idk::ECS*> m_scenes;

    idk::GenericLoader<idk::Game> *m_gameloader;
    idk::Game           *m_game     = nullptr;

    idk::Window         *m_win      = nullptr;
    idk::GLContext      *m_gl       = nullptr;

    idk::Engine         *m_engine   = nullptr;
    idk::IO             *m_io       = nullptr;
    idk::Audio          *m_audio    = nullptr;

    idk::RenderEngine   *m_renderer = nullptr;
    idk::EventEmitter<std::string, void*> *m_events = nullptr;


public:
    EngineMemory memory;

                         EngineAPI( const std::vector<std::string>&, const std::string&, int, int );

    void                 update( float dt );
    void                 updateScenes();
    float                dtime() { return m_dt; };

    idk::Window         &getWindow()     { return *m_win;      };
    idk::GLContext      &getGL()         { return *m_gl;       };
    idk::Engine         &getEngine()     { return *m_engine;   };
    idk::ECS            &getECS()        { return getScene("DefaultScene"); };
    idk::IO             &getIO()         { return *m_io;       };
    idk::Audio          &getAudio()      { return *m_audio;    };
    idk::RenderEngine   &getRenderer()   { return *m_renderer; };
    auto                &getEvents()     { return *m_events;   };
    idk::Game           *getGame()       { return  m_game;     };

    /**
     * Create a separate, isolated ECS which is not destroyed on reload.
     * Useful for persistent objects like UI.
     * - Automatically registers IconCmp, TransformCmp and TransformSys.
     * - Automatically calls ECS::update each frame.
     */
    idk::ECS            *createScene( const std::string &name );
    idk::ECS            &getScene( const std::string &name = "DefaultScene" );
    bool                 deleteScene( const std::string &name );
    const auto &         getScenes() { return m_scenes; };

    void                 reloadEngine();
    void                 reloadECS( bool now=false );
    void                 reloadGL();
    void                 reloadRenderer();
    void                 reloadGame( bool now=false );
    void                 reloadAll();

    bool                 running();
    void                 shutdown();

};
