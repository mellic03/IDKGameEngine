#include "../EditorUI.hpp"
#include "EditorUI-tabs.hpp"

#include "../idkui_components.hpp"
#include "../common/idk_imgui_helpmarker.hpp"



static void
idk_RemoveComponentPopup( idk::ECS &ecs, int obj_id, size_t component )
{
    if (ecs.hasComponent(obj_id, component) == false)
    {
        return;
    }

    std::string name  = ecs.getComponentArray(component)->getName();
    std::string label = "Remove " + name + " component";

    if (ImGui::MenuItem(label.c_str()))
    {
        ecs.removeComponent(obj_id, component);
    }
}


static void
idk_AddComponentPopup( idk::ECS &ecs, int obj_id )
{
    ImGui::Text("Add Component");
    ImGui::Separator();

    auto &categories = ecs.getComponentCategories();

    if (ImGui::BeginTable("Add Component Table", categories.size()))
    {
        ImGui::TableNextRow();

        for (auto &[category, keys]: categories)
        {
            ImGui::TableNextColumn();

            ImGui::Text(category.c_str());
            ImGui::Separator();

            for (auto &[name, C]: ecs.getComponentArraysByCategory(category))
            {
                if (ecs.hasComponent(obj_id, C->getKey()) == false)
                {
                    if (ImGui::MenuItem(name.c_str()))
                    {
                        ecs.giveComponent(obj_id, C->getKey());
                    }
                }
            }

        }
        ImGui::EndTable();
    }

}



void
EditorUI_MD::_tab_inspect( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    ImGui::Begin("Inspect Object");

    if (obj_id == -1 || ecs.gameObjectExists(obj_id) == false)
    {
        ImGui::End();
        return;
    }

    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();
    
    static size_t component = 0;
    static bool open_popup = false;

    std::string name = ecs.getGameObjectName(obj_id);
    if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        ecs.setGameObjectName(obj_id, name);
    }
    ImGui::SameLine();

    ImGui::Text(("Object ID: " + std::to_string(obj_id)).c_str());
    // ImGui::Text("REE");
    ImGui::SameLine();
    ImGui::Checkbox("Persistent", ecs.getGameObjectPersistency(obj_id));
    ImGui::Separator();


    if (idkImGui::splitWindow_begin("Inspect Object"))
    {
        ImGui::BeginChild("Upper", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    
        for (auto &[key, C]: ecs.getComponentArrays())
        {
            bool selected = (component == key);

            if (ecs.hasComponent(obj_id, key) == false)
            {
                continue;
            }

            std::string label = (C.get())->getName();
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
            idk_AddComponentPopup(ecs, obj_id);
            ImGui::EndPopup();
        }


        if (open_popup)
        {
            ImGui::OpenPopup("Remove Component");
            open_popup = false;
        }

        if (ImGui::BeginPopup("Remove Component"))
        {
            idk_RemoveComponentPopup(ecs, obj_id, component);
            ImGui::EndPopup();
        }


        idkImGui::splitWindow_split();


        if (ecs.hasComponent(obj_id, component))
        {
            ImGui::BeginChild("Component");

            (idk::ECS::getUserCallback(component))(api, ecs, obj_id);

            // auto *CA = ecs.getComponentArray(component);
            // CA->userCallback(api, ecs, obj_id);

            ImGui::EndChild();
        }

        idkImGui::splitWindow_end();
    }


    ImGui::End();
}

