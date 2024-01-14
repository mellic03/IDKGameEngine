#include "../EditorUI.hpp"
#include "../common/idk_imgui_extra.hpp"




static void
menubar_save( idk::EngineAPI &api )
{
    if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save", "CTRL + S"))
    {

    }
}


void
EditorUI_Module::_menubar( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    static bool popup_open = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "CTRL + N"))
            {

            }

            // menubar_save(api);

            if (ImGui::MenuItem(ICON_FA_ID_CARD " Save As", "CTRL + SHFT + S"))
            {
                popup_open = true;
            }

            if (ImGui::MenuItem("Load", "CTRL + L"))
            {
                
            }

            ImGui::Separator();
            ImGui::EndMenu();
        }

        _menubar_settings(api);

        if (ImGui::BeginMenu("Scene"))
        {
            ImGui::EndMenu();
        }

        float fps = ImGui::GetIO().Framerate;
        ImGui::Text("Framerate: %f", fps);

        ImGui::EndMainMenuBar();
    }


    static std::string selection = "";

    if (popup_open)
    {
        ImGui::OpenPopup("File Selection");
    }

    if (idkImGui::fileSelectPopup("File Selection", popup_open, "./", selection))
    {
        std::cout << "saving to " << selection << "\n";
    }

}

