#include "../EditorUI.hpp"
#include "../common/idk_imgui_extra.hpp"

#include <IDKIO/IDKIO.hpp>



static void
menubar_save( idk::EngineAPI &api )
{
    if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save", "CTRL + S"))
    {

    }
}


static void
menubar_file( idk::EngineAPI &api, bool &popup_save, bool &popup_load )
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

        // if (ImGui::MenuItem("Reload Modules", "F5"))
        // {
        //     api.getEngine().reloadModules();
        // }

        ImGui::Separator();
        ImGui::EndMenu();
    }
}


static void
menubar_engine( idk::EngineAPI &api )
{
    if (ImGui::BeginMenu("Engine"))
    {
        if (ImGui::MenuItem("Reload game"))     api.reloadGame();
        if (ImGui::MenuItem("Reload ECS"))      api.reloadECS();
        if (ImGui::MenuItem("Reload renderer")) api.reloadRenderer();

        ImGui::EndMenu();
    }
}



void
EditorUI_MD::_menubar( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &io     = api.getIO();
    auto &ren    = api.getRenderer();

    static bool popup_save = false;
    static bool popup_load = false;

    if (ImGui::BeginMainMenuBar())
    {
        menubar_file(api, popup_save, popup_load);
        _menubar_settings(api);
        menubar_engine(api);

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
        api.getECS().save(selection);
        // idk::ECS2::save(selection);
        std::cout << "Saved scene to " << selection << "\n";
    }

    if (popup_load)
    {
        ImGui::OpenPopup("File Load");
    }

    if (idkImGui::fileSelectPopup("File Load", popup_load, "./", selection))
    {
        api.getECS().load(selection);
        // idk::ECS2::load(selection);
        std::cout << "Loaded scene from " << selection << "\n";
    }
}

