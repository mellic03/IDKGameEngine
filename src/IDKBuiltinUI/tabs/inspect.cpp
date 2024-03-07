#include "../EditorUI.hpp"
#include "EditorUI-tabs.hpp"

#include "../idkui_components.hpp"
#include "../common/idk_imgui_helpmarker.hpp"



static void
idk_RemoveComponentPopup( idk::ecs::ECS &ecs, int obj_id, int component )
{
    std::string name  = ecs.getComponentArray(component)->name();
    std::string label = "Remove " + name + " component";

    if (ImGui::MenuItem(label.c_str()))
    {
        ecs.removeComponent(obj_id, component);
    }
}


static void
idk_AddComponentPopup( idk::ecs::ECS &ecs, int obj_id )
{
    ImGui::Text("Add Component");
    ImGui::Separator();

    for (int i=0; i<ecs.numComponents(); i++)
    {
        if (ecs.hasComponent(obj_id, i) == false)
        {
            idk::string name = ecs.getComponentArray(i)->name();
            if (ImGui::MenuItem(name.c_str()))
            {
                ecs.giveComponent(obj_id, i);
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
    auto &ecs    = api.getECS();

    static int component = -1;
    static bool open_popup = false;


    std::string name = ecs.getGameObjectName(obj_id);
    if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        ecs.setGameObjectName(obj_id, name);
    }


    if (idkImGui::splitWindow_begin("Inspect Object"))
    {
        ImGui::BeginChild("Upper", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    
        for (int i=0; i<ecs.numComponents(); i++)
        {
            bool selected = component == i;

            if (ecs.hasComponent(obj_id, i) == false)
            {
                continue;
            }

            idk::string label = ecs.getComponentArray(i)->name();
            if (ImGui::Selectable(label.c_str(), &selected))
            {
                component = i;
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
            auto *CA = ecs.getComponentArray(component);
            CA->drawCallback(api, obj_id);
            ImGui::EndChild();
        }

        idkImGui::splitWindow_end();
    }


    ImGui::End();
}

