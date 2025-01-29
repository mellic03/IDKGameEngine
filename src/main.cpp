#include <libidk/idk_platform.hpp>
#include <libidk/idk_api_loader.hpp>
#include <libidk/idk_threadpool.hpp>
#include <libidk/idk_module.hpp>
#include <libidk/idk_game.hpp>
#include <libidk/idk_string.hpp>
#include <libidk/idk_print.hpp>
#include <libidk/idk_log.hpp>
#include <libidk/idk_random.hpp>

#include <IDKGameEngine/IDKengine.hpp>
#include <IDKECS/IDKECS.hpp>

#include <IDKEvents/IDKEvents.hpp>
#include <IDKIO/IDKIO.hpp>

#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKBuiltinUI/EditorUI.hpp>

#include <IDKGameEngine/idk_engine_api.hpp>
#include <IDKGraphics/terrain/terrain.hpp>

#include <filesystem>
#include <iomanip>



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
    srand(clock());

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
    // Load engine code
    // -----------------------------------------------------------------------------------------
    idk::AudioSystem::init();
    idk::ThreadPool::init(arg_threads);

    std::string game_path = (arg_load_game) ? arg_game : "libgame.so";
    idk::EngineAPI api(args, game_path, 4, 6);
    // -----------------------------------------------------------------------------------------

    IDK_GLCALL(
        glDebugMessageControl(
            GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE
        );
    )

    idk::gl::enable(GL_DEBUG_OUTPUT);
    idk::gl::enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    IDK_GLCALL( glDebugMessageCallback(message_callback, nullptr); )


    // Run all scripts found in IDKGE/init
    // -----------------------------------------------------------------------------------------
    for (auto dir: std::filesystem::directory_iterator("./init/"))
    {
        auto path = dir.path(); path.replace_extension();
        auto name = path.string();
        idk::RuntimeScript script(name, false);
        script.execute(api, nullptr);
    }
    // -----------------------------------------------------------------------------------------


    // Main loop
    // -----------------------------------------------------------------------------------------
    uint64_t a = SDL_GetTicks64();
    uint64_t b = SDL_GetTicks64();
    uint64_t delta;
    float    dt;

    api.getRenderer().beginFrame();
    api.getRenderer().endFrame(0.0001);
    idk::TerrainRenderer::generateTerrain();

    while (api.running())
    {
        a = SDL_GetTicks64();
        delta = a - b;

        float dt = float(delta) / 1000.0f;
              dt = glm::clamp(dt, 0.0001f, 1.0f);

        // if (dt < 1.0f / 60.0f)
        // {
        //     continue;
        // }

        api.update(dt);
        api.updateScenes();

        auto *game   = api.getGame();
        auto &engine = api.getEngine();
        auto &io     = api.getIO();
        auto &ecs    = api.getECS();
        auto &ren    = api.getRenderer();

        io.update(delta);
        idk::Events::update();

        engine.beginFrame(api, dt);
        game->mainloop(api);
        engine.endFrame(api);
    
        idk::ThreadPool::update();
        idk::Logger::print();
        b = a;


        if (io.windowEvent(idk::IO::WIN_EXIT))
        {
            api.shutdown();
        }
    }
    // -----------------------------------------------------------------------------------------


    LOG_INFO() << "Main loop terminated, writing log to file";

    idk::Logger::write();


    return 0;
}


