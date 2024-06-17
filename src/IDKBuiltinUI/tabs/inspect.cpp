#include "../EditorUI.hpp"
#include "EditorUI-tabs.hpp"

#include "../idkui_components.hpp"
#include "../common/idk_imgui_helpmarker.hpp"



static void
idk_RemoveComponentPopup( int obj_id, size_t component )
{
    if (idk::ECS2::hasComponent(obj_id, component) == false)
    {
        return;
    }

    std::string name  = idk::ECS2::getComponentArray(component)->getName();
    std::string label = "Remove " + name + " component";

    if (ImGui::MenuItem(label.c_str()))
    {
        idk::ECS2::removeComponent(obj_id, component);
    }
}


static void
idk_AddComponentPopup( int obj_id )
{
    ImGui::Text("Add Component");
    ImGui::Separator();

    for (auto &[key, C]: idk::ECS2::getComponentArrays())
    {
        if (idk::ECS2::hasComponent(obj_id, key) == false)
        {
            std::string name = C->getName();

            if (ImGui::MenuItem(name.c_str()))
            {
                idk::ECS2::giveComponent(obj_id, key);
            }
        }
    }
}



void
EditorUI_MD::_tab_inspect( idk::EngineAPI &api, int obj_id )
{
    ImGui::Begin("Inspect Object");

    if (obj_id == -1)
    {
        ImGui::End();
        return;
    }

    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();
    

    static size_t component = 0;
    static bool open_popup = false;


    std::string name = idk::ECS2::getGameObjectName(obj_id);
    if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        idk::ECS2::setGameObjectName(obj_id, name);
    }


    if (idkImGui::splitWindow_begin("Inspect Object"))
    {
        ImGui::BeginChild("Upper", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    
        for (auto &[key, C]: idk::ECS2::getComponentArrays())
        {
            bool selected = (component == key);

            if (idk::ECS2::hasComponent(obj_id, key) == false)
            {
                continue;
            }

            std::string label = C->getName();
            if (ImGui::Selectable(label.c_str(), &selected))
            {
                component = key;
            }

            if (ImGui::IsItemClicked(1))
            {
                open_popup = true;
            }
        }

        ImGui::EndChild();

        if (ImGui::Button(ICON_FA_PLUS "Add"))
        {
            ImGui::OpenPopup("Add Component");
        }

        if (ImGui::BeginPopup("Add Component"))
        {
            idk_AddComponentPopup(obj_id);
            ImGui::EndPopup();
        }


        if (open_popup)
        {
            ImGui::OpenPopup("Remove Component");
            open_popup = false;
        }

        if (ImGui::BeginPopup("Remove Component"))
        {
            idk_RemoveComponentPopup(obj_id, component);
            ImGui::EndPopup();
        }


        idkImGui::splitWindow_split();



        if (idk::ECS2::hasComponent(obj_id, component))
        {
            ImGui::BeginChild("Component");

            std::string label = "Object ID: " + std::to_string(obj_id);
            ImGui::Text(label.c_str());
            ImGui::Separator();
            ImGui::Checkbox("Persistent", idk::ECS2::getGameObjectPersistency(obj_id));
            ImGui::Spacing();
            ImGui::Separator();

            auto *CA = idk::ECS2::getComponentArray(component);
            CA->userCallback(api, obj_id);

            ImGui::EndChild();
        }

        idkImGui::splitWindow_end();
    }


    ImGui::End();
}

