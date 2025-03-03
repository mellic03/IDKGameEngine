#include "EditorUI.hpp"
#include <IDKIO/IDKIO.hpp>

#include <idk_imgui/imgui.hpp>
#include <idk_imgui/imguizmo.hpp>
#include <idk_imgui/imnodes.hpp>
#include <libidk/idk_export.hpp>
#include <libidk/idk_log.hpp>

#include "common/idk_imgui_assetbrowser.hpp"
#include "tabs/node-editor.hpp"

#include "theme.hpp"
#include <filesystem>
namespace fs = std::filesystem;


static void
ImGui_SDL2_OpenGL_init( ImGuiContext *ctx, SDL_Window *win, SDL_GLContext gl )
{
    // LOG_INFO("EditorUI_MD::ImGui_SDL2_OpenGL_init");
    LOG_INFO("");

    ImGui::SetCurrentContext(ctx);

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigDockingTransparentPayload = true;

    ImGui::StyleColorsLight();
    idkImGui_theme();

    IDK_ASSERT(
        "ImGui_ImplSDL2_InitForOpenGL failure",
        ImGui_ImplSDL2_InitForOpenGL(win, gl) == true
    );

    IDK_ASSERT(
        "ImGui_ImplOpenGL3_Init failure",
        ImGui_ImplOpenGL3_Init("#version 460") == true
    );

}


static ImGuiContext *main_ctx;
static ImGuiContext *popout_ctx;
static SDL_Window   *popout_win;
static int m_pollEventCallback;
static idk::EngineAPI *api_ptr;


void
EditorUI_MD::init( idk::EngineAPI &api )
{
    LOG_INFO("");
    LOG_INFO("m_tabs.size(): {}", m_tabs.size());

    api_ptr = &api;
    auto &engine   = api.getEngine();
    auto &ren      = api.getRenderer();

    registerDrawComponents();

    IMGUI_CHECKVERSION();
    main_ctx = ImGui::CreateContext();
    ImGui_SDL2_OpenGL_init(main_ctx, api.getWindow().sdl_win, api.getGL().sdl_glctx);

    ImGui::SetCurrentContext(main_ctx);
    ImGuiIO& io = ImGui::GetIO();

    m_pollEventCallback = api.getIO().onPollEvent(
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
EditorUI_MD::deinit()
{
    // LOG_INFO("EditorUI_MD::deinit");
    LOG_INFO("");

    this->clearDrawComponents();
    api_ptr->getIO().removeCallback(m_pollEventCallback);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}




void
EditorUI_MD::stage_B( idk::EngineAPI &api )
{
    api_ptr = &api;
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

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    // ImGui::GetIO().WantCaptureMouse = !api.getIO().mouseCaptured();
    if (api.getIO().mouseCaptured())
    {
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
    }
    else
    {
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }

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
    if (m_ecs)
    {
        ImGui::Begin("Viewport");
        this->_tab_viewport(api, *m_ecs);
        ImGui::End();
    }

    this->_tab(api);
    for (auto &callback: m_tabs)
    {
        callback(api);
    }


    constexpr int NUM_ASSET_BROWSERS = 2;

    static fs::directory_entry dir_1(fs::absolute("assets/models"));
    static fs::directory_entry dir_2(fs::absolute("assets/audio"));

    idkImGui::AssetBrowser("Asset Browser ##A", dir_1);
    idkImGui::AssetBrowser("Asset Browser ##B", dir_2);
    ImGui::End();

    ImGui::Render();
    ImGui::EndFrame();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#undef ECS_REGISTER_COMPONENT
