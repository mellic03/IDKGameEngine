#include "./EditorUI.hpp"

#include <IDKIO/IDKIO.hpp>

#include <idk_imgui/imgui.hpp>
#include <idk_imgui/imguizmo.hpp>
#include <idk_imgui/imnodes.hpp>
#include <libidk/idk_export.hpp>

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


static ImGuiContext *main_ctx;
static ImGuiContext *popout_ctx;
static SDL_Window   *popout_win;


void
idk::EditorUI::init( idk::EngineAPI &api )
{
    auto &engine   = api.getEngine();
    auto &IO       = api.getIO();
    auto &ren      = api.getRenderer();

    // registerDrawComponents();

    IMGUI_CHECKVERSION();
    main_ctx = ImGui::CreateContext();
    ImGui_SDL2_OpenGL_init(main_ctx, api.getWindow().sdl_win, api.getGL().sdl_ctx);

    ImGui::SetCurrentContext(main_ctx);
    ImGuiIO& io = ImGui::GetIO();

    IO.onPollEvent(
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
idk::EditorUI::deinit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}


void
idk::EditorUI::registerECS( idk::EngineAPI &api, idk::ECS &ecs )
{
    
}





void
idk::EditorUI::stage_B( idk::EngineAPI &api )
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

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();


    if (m_demowindow)
    {
        ImGui::ShowDemoWindow(&m_demowindow);
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
        // this->_tab_viewport(api, *m_ecs);
        ImGui::End();
    }

    // this->_tab(api);
    for (auto &callback: m_callbacks)
    {
        callback(api);
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#undef ECS_REGISTER_COMPONENT
