#include "../EditorUI.hpp"
#include "EditorUI-tabs.hpp"

#include "../idkui_components.hpp"




static void
tab_inspect_component( idk::EngineAPI &api, int object_id, idk::ComponentSystem *CS )
{
    if (CS == nullptr)
    {
        return;
    }

    std::string label = CS->name() + " component";
    ImGui::BeginChild(label.c_str());

    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    CS->onObjectSelection(object_id);


    ImGui::EndChild();
}



void
EditorUI_Module::_tab_inspect( idk::EngineAPI &api, int object_id )
{
    ImGui::Begin("Inspect Object");

    if (object_id == -1)
    {
        ImGui::End();
        return;
    }

    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    std::string label = "Object ID: " + std::to_string(object_id);
    ImGui::Text(label.c_str());

    static idk::ComponentSystem *CS_ptr = nullptr;
    static int stored_id = 0;

    int cs_id = (CS_ptr) ? CS_ptr->ID() : -1;

    if (object_id != stored_id)
    {
        stored_id = object_id;
    
        if (CS_ptr && engine.hasComponent(object_id, CS_ptr->ID()) == false)
        {
            CS_ptr = nullptr;
        }
    }


    if (idkImGui::splitWindow_begin("Inspect Component"))
    {
        for (auto &CS: engine.getComponentSystems())
        {
            const std::string &name = CS->name();
            const int CS_id = CS->ID();

            if (engine.hasComponent(object_id, CS_id))
            {
                if (ImGui::MenuItem(name.c_str()))
                {
                    CS_ptr = CS;
                }
            }
        }

        idkImGui::splitWindow_split();
        tab_inspect_component(api, object_id, CS_ptr);
        idkImGui::splitWindow_end();
    }



    ImGui::End();
}

