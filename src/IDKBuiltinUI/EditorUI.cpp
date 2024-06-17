#include "EditorUI.hpp"

#include <IDKEvents/IDKEvents.hpp>

#include <idk_imgui/imgui.hpp>
#include <idk_imgui/imguizmo.hpp>
#include <idk_imgui/imnodes.hpp>
#include <libidk/idk_export.hpp>

#include "common/idk_imgui_assetbrowser.hpp"
#include "tabs/node-editor.hpp"

#include "theme.hpp"
#include <filesystem>
namespace fs = std::filesystem;


static void
ImGui_SDL2_OpenGL_init( ImGuiContext *ctx, SDL_Window *win, SDL_GLContext gl )
{
    ImGui::SetCurrentContext(ctx);

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigDockingTransparentPayload = true;

    ImGui::StyleColorsLight();
    idkImGui_theme();

    ImGui_ImplSDL2_InitForOpenGL(win, gl);
    ImGui_ImplOpenGL3_Init("#version 460");

}




void
EditorUI_MD::deinit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}


static ImGuiContext *main_ctx;
static ImGuiContext *popout_ctx;
static SDL_Window   *popout_win;




void
EditorUI_MD::init( idk::EngineAPI &api )
{
    auto &engine   = api.getEngine();
    
    auto &ren      = api.getRenderer();
    auto &eventsys = api.getEventSys();


    this->registerDrawComponents(api);

    IMGUI_CHECKVERSION();
    main_ctx = ImGui::CreateContext();
    ImGui_SDL2_OpenGL_init(main_ctx, ren.getWindow(), ren.getGLContext());


    // popout_ctx = ImGui::CreateContext();
    // popout_win = SDL_CreateWindow("Pop out", 0, 0, 512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    // SDL_GL_SetSwapInterval(0);
    // ImGui_SDL2_OpenGL_init(popout_ctx, ren.getWindow(), ren.getGLContext());


    ImGui::SetCurrentContext(main_ctx);
    ImGuiIO& io = ImGui::GetIO();


    eventsys.onSDLPollEvent(
        [](SDL_Event *event)
        {
            ImGui::SetCurrentContext(main_ctx);
            ImGui_ImplSDL2_ProcessEvent(event);
        }
    );


    for (size_t i=0; i<NUM_FONTS; i++)
    {
        float font_size = FONT_SIZES[i];

        std::string path = "IDKGE/resources/fonts/Ubuntu/"
                         + std::to_string(int(font_size))
                         + "/Ubuntu-Medium.ttf";

        m_fonts[i] = io.Fonts->AddFontFromFileTTF(path.c_str(), font_size);


        // Icons
        // -------------------------------------------------------------------------------------
        static const ImWchar icon_ranges[] = { IDK_FONT_ICON_MIN, IDK_FONT_ICON_MAX, 0 };
        float icon_size = font_size * IDK_FONT_ICON_SCALE_FACTOR;
        float icon_size_2 = font_size * IDK_FONT_ICON_SCALE_FACTOR2;
        
        ImFontConfig config;
        config.MergeMode  = true;
        config.PixelSnapH = true;
        config.GlyphMinAdvanceX = 1.5f * icon_size;

        io.Fonts->AddFontFromFileTTF(IDK_FONT_ICON_FILEPATH, icon_size, &config, icon_ranges);
        // -------------------------------------------------------------------------------------
    }

    io.FontDefault = m_fonts[1];

    ImGuizmo::GetStyle().RotationLineThickness = 6.0f;
}



void
EditorUI_MD::stage_B( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();



    ImGuiWindowFlags windowflags = ImGuiWindowFlags_NoResize
                                 | ImGuiWindowFlags_NoMove
                                 | ImGuiWindowFlags_NoCollapse
                                 | ImGuiWindowFlags_NoFocusOnAppearing
                                 | ImGuiWindowFlags_AlwaysVerticalScrollbar
                                 | ImGuiWindowFlags_MenuBar
                                 | ImGuiWindowFlags_NoTitleBar
                                 | ImGuiWindowFlags_NoBringToFrontOnFocus;




    // static bool pop_out = false;

    // if (api.getEventSys().keylog().keyTapped(idk::Keycode::F11))
    // {
    //     pop_out = !pop_out;
    // }

    // if (pop_out)
    // {
    //     SDL_GL_MakeCurrent(popout_win, ren.getGLContext());
    //     ImGui::SetCurrentContext(popout_ctx);


    //     int w, h;
    //     SDL_GetWindowSize(popout_win, &w, &h);
    //     ImGui::SetNextWindowPos({0, 0});
    //     ImGui::SetNextWindowSize(ImVec2(float(w), float(h)));
    
    //     ImGui_ImplOpenGL3_NewFrame();
    //     ImGui_ImplSDL2_NewFrame();
    //     ImGui::NewFrame();
    //     ImGuizmo::BeginFrame();

    //     // ImGui::Begin("Root", nullptr, windowflags);
    //     this->_tab_viewport(api);
    //     // ImGui::End();

    //     ImGui::Render();
    //     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //     SDL_GL_SwapWindow(popout_win);
    //     SDL_GL_MakeCurrent(ren.getWindow(), ren.getGLContext());
    //     ImGui::SetCurrentContext(main_ctx);
    // }


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();


    api.getEventSys().onDropFile(".idkvi", [&api](const char *path)
    {
        auto &ren = api.getRenderer();
        ren.loadModel(path);
    });

    // engine.getCS<idk::Icon_CS>().setDefaultIcon(ICON_FA_BOX_OPEN);

    if (m_show_ImGui_demo)
    {
        ImGui::ShowDemoWindow(&m_show_ImGui_demo);
    }

    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("Root", nullptr, windowflags);
    ImGui::DockSpace(ImGui::GetID("Root-Dockspace"));


    this->_menubar(api);

    // if (pop_out == false)
    {
        ImGui::Begin("Viewport");
        this->_tab_viewport(api);
        ImGui::End();
    }

    // {
    //     ImGui::Begin("VXGI");

    //     GLuint texture = ren.m_vxgi_buffer.attachments[0];
    //     ImGui::Image(
    //         *(ImTextureID *)(void *)(&texture),
    //         ImVec2(256, 256),
    //         ImVec2(0.0f, 1.0f),
    //         ImVec2(1.0f, 0.0f)
    //     );

    //     texture = ren.m_vxgi_buffer.depth_attachment;
    //     ImGui::Image(
    //         *(ImTextureID *)(void *)(&texture),
    //         ImVec2(256, 256),
    //         ImVec2(0.0f, 1.0f),
    //         ImVec2(1.0f, 0.0f)
    //     );

    //     ImGui::End();
    // }


    this->_tab(api);

    static bool open = true;
    static std::string selection = "";


    constexpr int NUM_ASSET_BROWSERS = 2;

    static fs::directory_entry dir_1(fs::absolute("assets/models"));
    static fs::directory_entry dir_2(fs::absolute("assets/audio"));

    idkImGui::AssetBrowser("Asset Browser ##A", dir_1);
    idkImGui::AssetBrowser("Asset Browser ##B", dir_2);


    ImGui::End();

    // idk_NodeEditor(api);

    ImGui::Render();

    // if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    // {
        // ImGui::UpdatePlatformWindows();
    //     ImGui::RenderPlatformWindowsDefault();
    // }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


#undef ECS_REGISTER_COMPONENT
