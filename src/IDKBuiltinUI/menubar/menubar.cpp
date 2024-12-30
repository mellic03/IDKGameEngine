#include "../EditorUI.hpp"
#include "../common/idk_imgui_extra.hpp"

#include <IDKGameEngine/packager.hpp>
#include <IDKIO/IDKIO.hpp>



static void
menubar_save( idk::EngineAPI &api )
{
    if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save", "CTRL + S"))
    {

    }
}


void
EditorUI_MD::_menubar( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    if (idkio::keyTapped(idk::Keycode::F5))
    {
        engine.reloadModules();
    }

    static bool popup_save = false;
    static bool popup_load = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "CTRL + N"))
            {
                
            }

            if (ImGui::MenuItem("Save As " ICON_FA_FILE_EXPORT, "CTRL + SHFT + S"))
            {
                popup_save = true;
            }

            if (ImGui::MenuItem("Load " ICON_FA_FILE_IMPORT, "CTRL + L"))
            {
                popup_load = true;
            }

            if (ImGui::BeginMenu("Project"))
            {
                if (ImGui::MenuItem("Build"))
                {
                    api.getPackager().build();
                }
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Reload Modules", "F5"))
            {
                engine.reloadModules();
            }

            ImGui::Separator();
            ImGui::EndMenu();
        }

        _menubar_settings(api);


        float fps = ImGui::GetIO().Framerate;
        ImGui::Text("Framerate: %f", fps);

        ImGui::EndMainMenuBar();
    }


    static std::string selection = "";

    if (popup_save)
    {
        ImGui::OpenPopup("File Save");
    }

    if (idkImGui::fileSelectPopup("File Save", popup_save, "./", selection))
    {
        idk::ECS2::save(selection);
        std::cout << "Saved scene to " << selection << "\n";
    }

    if (popup_load)
    {
        ImGui::OpenPopup("File Load");
    }

    if (idkImGui::fileSelectPopup("File Load", popup_load, "./", selection))
    {
        idk::ECS2::load(selection);
        std::cout << "Loaded scene from " << selection << "\n";
    }
}

