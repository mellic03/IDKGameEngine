#include "../EditorUI.hpp"
#include "EditorUI-tabs.hpp"

#include "../idkui_components.hpp"
#include "../common/idk_imgui_helpmarker.hpp"




static void
tab_inspect_component( idk::EngineAPI &api, int object_id, int CS_id )
{
    auto &engine = api.getEngine();

    idk::ComponentSystem *CS = engine.getCS(CS_id);
    std::string label = CS->name() + " component";

    ImGui::BeginChild(label.c_str());
    CS->onObjectSelection(object_id);
    ImGui::EndChild();
}


static void
idk_tab_ComponentContextMenu( int obj_id, int &cs_id, idk::Engine &engine )
{
    std::string label = "Remove " + engine.getCS(cs_id)->name() + " component";

    if (ImGui::MenuItem(label.c_str()))
    {
        engine.removeComponent(obj_id, cs_id);
        cs_id = -1;
    }
}


static void
idk_tab_giveComponentMenu( int obj_id, idk::Engine &engine )
{
    ImGui::Text("Add Component");
    ImGui::Separator();

    for (auto *CS: engine.getComponentSystems())
    {
        int CS_id = CS->ID();

        if (engine.hasComponent(obj_id, CS_id) == false)
        {
            if (ImGui::MenuItem(CS->name().c_str()))
            {
                engine.giveComponent(obj_id, CS_id);
            }
        }
    }
}



void
EditorUI_Module::_tab_inspect( idk::EngineAPI &api, int obj_id )
{
    ImGui::Begin("Inspect Object");

    if (obj_id == -1)
    {
        ImGui::End();
        return;
    }

    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    static int selected_CS  = -1;
    static int selected_obj = 0;

    if (obj_id != selected_obj)
    {
        selected_obj = obj_id;

        if (selected_CS != -1)
        {
            if (engine.hasComponent(obj_id, selected_CS) == false)
            {
                selected_CS = -1;
            }
        }
    }


    std::string label = "Object ID: " + std::to_string(obj_id);
    ImGui::Text(label.c_str());

    std::string name = engine.getGameObjectName(obj_id);
    if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        engine.getGameObjectName(obj_id) = name;
    }


    if (idkImGui::splitWindow_begin("Inspect Component"))
    {
        ImGui::BeginChild("Upper", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
        for (auto *CS: engine.getComponentSystems())
        {
            const std::string &name = CS->name();
            bool selected = CS->ID() == selected_CS;

            if (engine.hasComponent(obj_id, CS->ID()))
            {
                if (ImGui::Selectable(name.c_str(), &selected, ImGuiSelectableFlags_SelectOnClick))
                {
                    selected_CS = CS->ID();
                }

                if (ImGui::IsItemClicked(1))
                {
                    selected_CS = CS->ID();
                    ImGui::OpenPopup("CSContext");
                }
            }
        }

        if (ImGui::BeginPopup("CSContext"))
        {
            idk_tab_ComponentContextMenu(obj_id, selected_CS, engine);
            ImGui::EndPopup();
        }

        ImGui::EndChild();

        if (ImGui::BeginPopup("my popup"))
        {
            idk_tab_giveComponentMenu(obj_id, engine);
            ImGui::EndPopup();
        }

        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("my popup");
        }


        idkImGui::splitWindow_split();
        if (selected_CS != -1)
        {
            tab_inspect_component(api, obj_id, selected_CS);
        }
        idkImGui::splitWindow_end();
    }



    ImGui::End();
}

