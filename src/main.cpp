#include <libidk/idk_platform.hpp>
#include <libidk/idk_api_loader.hpp>
#include <libidk/idk_module.hpp>
#include <libidk/idk_game.hpp>
#include <libidk/idk_string.hpp>
#include <libidk/idk_print.hpp>
#include <libidk/idk_log.hpp>

#include <IDKGameEngine/IDKengine.hpp>
#include <IDKECS/IDKECS.hpp>

#include <IDKEvents/IDKEvents.hpp>

#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKBuiltinUI/EditorUI.hpp>

#include <IDKGameEngine/idk_engine_api.hpp>
#include <IDKThreading/IDKThreading.hpp>

#include <IDKGraphics/UI/idk_ui.hpp>


#include <filesystem>



void
message_callback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                  GLchar const* message, void const* user_param )
{
	auto const src_str = [source]()
    {
		switch (source)
		{
            default:                                return "UNKNOWN";
            case GL_DEBUG_SOURCE_API:               return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     return "WINDOW SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER:   return "SHADER COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY:       return "THIRD PARTY";
            case GL_DEBUG_SOURCE_APPLICATION:       return "APPLICATION";
            case GL_DEBUG_SOURCE_OTHER:             return "OTHER";
		}
	}();

	auto const type_str = [type]()
    {
		switch (type)
		{
            default:                                return "UNKNOWN";
            case GL_DEBUG_TYPE_ERROR:               return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY:         return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER:              return "MARKER";
            case GL_DEBUG_TYPE_OTHER:               return "OTHER";
		}
	}();

	auto const severity_str = [severity]
    {
		switch (severity)
        {
            default:                                return "UNKNOWN";
		    case GL_DEBUG_SEVERITY_NOTIFICATION:    return "NOTIFICATION";
            case GL_DEBUG_SEVERITY_LOW:             return "LOW";
            case GL_DEBUG_SEVERITY_MEDIUM:          return "MEDIUM";
            case GL_DEBUG_SEVERITY_HIGH:            return "HIGH";
		}
	}();

    idk::Logger::print();

	std::cout << src_str << ", "
              << type_str << ", "
              << severity_str << ", "
              << id << ": "
              << message << '\n';
}



int main( int argc, char **argv )
{
    // Parse command-line arguments
    // -----------------------------------------------------------------------------------------
    std::vector<std::string> args;

    bool arg_load_modules = false;
    bool arg_load_idksc   = false;
    bool arg_load_game    = false;
    size_t arg_threads    = 2;

    std::string arg_idksc = "";
    std::string arg_game  = "";

    for (int i=1; i<argc; i++)
    {
        args.push_back(argv[i]);

        std::string arg = std::string(argv[i]);

        if (arg == "-lm" || arg == "--load-modules")
        {
            arg_load_modules = true;
        }

        else if (arg == "-ls" || arg == "--load-scene")
        {
            arg_load_idksc = true;
            arg_idksc = std::string(argv[i+1]);
        }

        else if (arg == "-lg" || arg == "--load-game")
        {
            arg_load_game = true;
            arg_game = std::string(argv[i+1]);
        }

        else if (arg == "--threads")
        {
            arg_threads = std::stoul(argv[i+1]);
        }
    }
    // -----------------------------------------------------------------------------------------


    // Load game code
    // -----------------------------------------------------------------------------------------
    std::string game_path = (arg_load_game) ? arg_game : "libgame.so";

    idk::GenericLoader<idk::Game> gameLoader(game_path);
    idk::Game *game = gameLoader.getInstance();
    // -----------------------------------------------------------------------------------------


    // Load engine code
    // // -----------------------------------------------------------------------------------------
    idk::EngineAPI api;
    api.init(game->getName(), 4, 6);

    auto &eventsys   = api.getEventSys();
    auto &audiosys   = api.getAudioSys();
    auto &engine     = api.getEngine();
    auto &ren        = api.getRenderer();

    idk::ThreadPool::init(arg_threads);
    // -----------------------------------------------------------------------------------------

    IDK_GLCALL(
        glDebugMessageControl(
            GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE
        );
    )

    idk::gl::enable(GL_DEBUG_OUTPUT);
    idk::gl::enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    IDK_GLCALL( glDebugMessageCallback(message_callback, nullptr); )



    // Setup resize and exit callbacks
    // -----------------------------------------------------------------------------------------
    auto resize_lambda = [&ren, &eventsys]()
    {
        auto winsize = eventsys.windowSize();
        ren.resize(winsize.x, winsize.y);
    };

    auto exit_lambda = [&engine]()
    {
        engine.shutdown();
    };

    eventsys.onWindowEvent(idk::WindowEvent::RESIZE, resize_lambda);
    eventsys.onWindowEvent(idk::WindowEvent::EXIT,   exit_lambda);
    // -----------------------------------------------------------------------------------------


    // Load built-in components + systems
    // -----------------------------------------------------------------------------------------
    idk::registerComponents();
    idk::registerSystems();
    // -----------------------------------------------------------------------------------------


    // Load modules from IDKGE/runtime/modules/
    // -----------------------------------------------------------------------------------------
    if (arg_load_modules)
    {
        std::filesystem::directory_iterator d_iter("IDKGE/runtime/modules/");

        for (auto dir: d_iter)
        {
            std::string name = dir.path().stem();
            std::string path = dir.path();

            engine.registerModule(name, path);
        }
    }
    // -----------------------------------------------------------------------------------------


    // idk::RenderOverlay test
    // -----------------------------------------------------------------------------------------
    ren.pushRenderOverlayFill(glm::vec3(0.0f), 0.0f, 6.25f, 0.25f);
    ren.pushRenderOverlay("IDKGE/resources/logo.jpg", 0.5f, 2.0f, 0.5f);
    // -----------------------------------------------------------------------------------------


    // Setup
    // -----------------------------------------------------------------------------------------
    game->registerModules(api);

    idk::ECS2::init(api);
    engine.initModules(api);

    if (arg_load_idksc)
    {
        idk::ECS2::load(arg_idksc);
        idk::ECS2::init(api);
    }
    idk::ECS2::update(api);


    game->setup(args, api);
    // -----------------------------------------------------------------------------------------



    // Main loop
    // -----------------------------------------------------------------------------------------
    uint64_t a = SDL_GetTicks64();
    uint64_t b = SDL_GetTicks64();
    uint64_t delta;

    while (engine.running())
    {
        a = SDL_GetTicks64();
        delta = a - b;

        float dt = float(double(delta) / 1000.0);
              dt = glm::clamp(dt, 0.0001f, 1.0f/30.0f);

        // if (dt < 1.0f / 60.0f)
        // {
        //     continue;
        // }

        eventsys.processKeyInput();
        eventsys.processMouseInput();
        eventsys.update();

        idk::ECS2::update(api);
        idk::Events::update();

        engine.beginFrame(api, dt);
        game->mainloop(api);
        engine.endFrame(api);
    
        idk::ThreadPool::update();

        idk::Logger::print();
        b = a;
    }
    // -----------------------------------------------------------------------------------------


    game->shutdown();

    LOG_INFO() << "Main loop terminated, writing log to file";

    idk::Logger::write();


    return 0;
}


