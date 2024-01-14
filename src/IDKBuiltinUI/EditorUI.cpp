#include "EditorUI.hpp"

// #include "tabs/EditorUI-tabs.hpp"
#include <IDKEvents/IDKEvents.hpp>

#include <idk_imgui/imgui.hpp>
#include <idk_imgui/imguizmo.hpp>
#include <idk_imgui/imnodes.hpp>


static ImNodesContext *context;

void
EditorUI_Module::init( idk::EngineAPI &api )
{
    auto &engine   = api.getEngine();
    auto &ren      = api.getRenderer();
    auto &eventsys = api.getEventSys();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigDockingTransparentPayload = true;

    ImGui::StyleColorsLight();

    ImGui_ImplSDL2_InitForOpenGL(
        ren.SDLWindow(),
        ren.SDLGLContext()
    );

    ImGui_ImplOpenGL3_Init("#version 440");

    eventsys.onSDLPollEvent(
        [](SDL_Event *event)
        {
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


    ImGuizmo::GetStyle().RotationLineThickness    = 6.0f;
    // context = ImNodes::CreateContext();
}


void
EditorUI_Module::stage_B( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();


    engine.getCS<idk::Icon_CS>().setDefaultIcon(ICON_FA_BOX_OPEN);

    if (m_show_ImGui_demo)
    {
        ImGui::ShowDemoWindow(&m_show_ImGui_demo);
    }

    ImGui::Begin("Editor Dockspace");
    ImGui::DockSpace(ImGui::GetID("Editor-Dockspace"));
    ImGui::End();

    this->_menubar(api);
    this->_tab(api);

    {
    // constexpr int STRIDE = 15;
    // int ID = 1;

    // static int num_links = 0;
    // static std::map<int, std::pair<int, int>> links;

    // ImNodes::BeginNodeEditor();

    // context->CurrentAttributeFlags = ImNodesAttributeFlags_EnableLinkDetachWithDragClick;

    // for (auto &[id, pair]: links)
    // {
    //     ImNodes::Link(id, pair.first, pair.second);
    // }

    // for (int i=0; i<5; i++)
    // {
    //     ImNodes::BeginNode(ID++);

    //     ImNodes::BeginNodeTitleBar();
    //     ImGui::Text("Node %d", i);
    //     ImNodes::EndNodeTitleBar();

    //     for (int j=0; j<3; j++)
    //     {
    //         ImNodes::BeginInputAttribute(ID++);
    //         ImGui::Text("Input %d", j);
    //         ImNodes::EndInputAttribute();
    //     }

    //     ImNodes::BeginOutputAttribute(ID++);
    //     ImGui::Text("Output");
    //     ImNodes::EndOutputAttribute();

    //     ImNodes::EndNode();
    // }
    // ImNodes::EndNodeEditor();

    // int link_id, start, end;
    // if (ImNodes::IsLinkCreated(&start, &end))
    // {
    //     links[num_links] = std::make_pair(start, end);
    //     num_links += 1;
    // }

    // if (ImNodes::IsLinkDestroyed(&link_id))
    // {
    //     std::cout << "Link " << link_id << "\n";
    //     links.erase(link_id);
    // }
    }


    ImGui::Render();

    // if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    // {
        // ImGui::UpdatePlatformWindows();
    //     ImGui::RenderPlatformWindowsDefault();
    // }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

