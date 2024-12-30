#pragma once

#include <map>
#include <set>
#include <deque>
#include <memory>
#include <vector>

#include <libidk/idk_module.hpp>
#include <libidk/idk_dynamiclib.hpp>

#include <cstdint>
#include <cstddef>
#include <unordered_map>


namespace idk { class Engine;       };
namespace idk { class RenderEngine; };
namespace idk { class EventSystem;  };


class idk::Engine
{
private:
    using ModuleLoader = idk::GenericLoader<idk::Module>;

    uint64_t                                    m_frame_start = 0;
    uint64_t                                    m_frame_end   = 0;
    float                                       m_frame_time  = 0.0001f;
    bool                                        m_running     = true;
    bool                                        m_reload      = false;

    std::vector<ModuleLoader>                   m_modules;

    void                                        _idk_modules_stage_A( idk::EngineAPI & );
    void                                        _idk_modules_stage_B( idk::EngineAPI & );
    void                                        _idk_modules_stage_C( idk::EngineAPI & );
    void                                        _reloadModules();


public:


    void                                        initModules( idk::EngineAPI & );
    void                                        reloadModules() { m_reload = true; };

    void                                        beginFrame  ( idk::EngineAPI&, float );
    void                                        endFrame    ( idk::EngineAPI& );

    bool                                        running() { return m_running; };
    void                                        shutdown();

    float                                       deltaTime() { return m_frame_time;       };
    float                                       frameRate() { return 1.0f / deltaTime(); };



    /**
     * @param filename filename excluding extension (.so/.dll)
     */
    // template <typename module_type> int         registerModule( const std::string &name );
    int                                         registerModule( const std::string &filename );

};

