#include "../EditorUI.hpp"
#include "../common/idk_imgui_extra.hpp"
#include <IDKGameEngine/IDKGameEngine.hpp>


void
EditorUI_MD::_tab_scripts( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    

    static bool open = false;
    static std::string selection = "";


    ImGui::Begin("Scripts");

    // for (auto &path: idk::ECS2::getSystem<idk::ScriptSys>().getScripts())
    // {
    //     if (ImGui::TreeNodeEx(path.c_str(), ImGuiTreeNodeFlags_Leaf))
    //     {
    //         if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    //         {
    //             std::string filepath = path;

    //             ImGui::SetDragDropPayload("SCRIPT_DRAG_DROP",
    //                 (const void *)(path.c_str()),
    //                 sizeof(char) * (filepath.length() + 1)
    //             );

    //             if (ImGui::IsDragDropActive())
    //             {
    //                 ImGui::Text(path.c_str());
    //             }
    //             ImGui::EndDragDropSource();
    //         }

    //         ImGui::TreePop();
    //     }

    //     // ImGui::SameLine();

    //     // if (ImGui::Button(ICON_FA_ARROWS_ROTATE "Reload"))
    //     // {
    //     //     script.reload();
    //     // }
    // }

    // ImGui::Separator();

    // if (ImGui::Button(ICON_FA_FOLDER_OPEN "Load"))
    // {
    //     open = true;
    //     ImGui::OpenPopup("File Load");
    // }

    // if (idkImGui::fileSelectPopup("File Load", open, "./", selection))
    // {
    //     idk::ECS2::getSystem<idk::ScriptSys>().loadScript(selection);
    // }


    ImGui::End();
}


