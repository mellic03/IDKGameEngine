#include "../EditorUI.hpp"
#include "EditorUI-tabs.hpp"



static void
idk_scene_treenode_drag_drop( int id, idk::Transform_CS &tCS )
{
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        ImGui::SetDragDropPayload("SCENE_HIERARCHY", reinterpret_cast<const void *>(&id), sizeof(int));
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        ImGui::BeginTooltip();
        ImGui::TextColored({255, 0, 0, 255}, "Parent");
        ImGui::EndTooltip();

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
        {
            IM_ASSERT(payload->DataSize == sizeof(int));
            int child_id = *reinterpret_cast<int *>(payload->Data);
            tCS.giveChild(id, child_id);
        }
        ImGui::EndDragDropTarget();
    }
}


void
EditorUI_Module::_tab_scene_treenode( idk::EngineAPI &api, int id )
{
    if (id == -1)
    {
        return;
    }

    auto &engine  = api.getEngine();
    auto &iCS     = engine.getCS<idk::Icon_CS>();
    auto &tCS     = engine.getCS<idk::Transform_CS>();


    std::string &name = engine.getGameObjectName(id);;
    std::string label = iCS.getIcon(id) + std::string(" ") + name;
    int flags  = ImGuiTreeNodeFlags_DefaultOpen;
        flags |= ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanFullWidth;

    if (tCS.getChildren(id).size() == 0)
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (id == m_selection.front())
    {
        flags |= ImGuiTreeNodeFlags_Selected;
    }


    ImGui::PushID(id);

    bool node_open = ImGui::TreeNodeEx(label.c_str(), flags);
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
        _select_object(id);
    }

    if (node_open)
    {
        if (ImGui::BeginPopupContextWindow("Object Context"))
        {
            ImGui::Text(engine.getGameObjectName(_get_selection()).c_str());
            ImGui::Separator();

            if (ImGui::MenuItem("Copy"))
            {
                engine.copyGameObject(_get_selection());
            }

            if (ImGui::MenuItem("Delete"))
            {
                engine.deleteGameObject(m_selection.front());
                m_selection.pop_front();
            }

            ImGui::EndPopup();
        }

        idk_scene_treenode_drag_drop(id, tCS);

        for (int child_id: tCS.getChildren(id))
        {
            if (child_id == 0)
            {
                continue;
            }
    
            _tab_scene_treenode(api, child_id);
        }

        ImGui::TreePop();
    }

    ImGui::PopID();
}


void rightwindow( idk::EngineAPI &api, int selected )
{
    // auto &engine = api.getEngine();
    // auto &nCS    = engine.getCS<idk::Name_CS>();

    // ImGui::BeginChild("LR", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    // std::string *text = &nCS.getName(selected);
    // ImGui::InputText("Name: ", text);
    // ImGui::EndChild();

    // if (idkImGui::splitWindow_begin("Details"))
    // {
    //     ImGui::Text("Object");

    //     idkImGui::splitWindow_split();

    //     if (ImGui::Button("Add Component"))
    //     {
    //         ImGui::OpenPopup("Add Component");
    //     }

    //     if (ImGui::BeginPopup("Add Component"))
    //     {
    //         ImGui::Text("Add Component");
    //         ImGui::Separator();

    //         for (int i=0; i<5; i++)
    //         {
    //             std::string label = engine.getCS(i)->name();

    //             if (ImGui::MenuItem(label.c_str()))
    //             {
    //                 engine.giveComponent(selected, i);
    //             }
    //         }

    //         ImGui::EndPopup();
    //     }

    //     idkImGui::splitWindow_end();
    // }
}


void
EditorUI_Module::_tab_scene_hierarchy( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    // auto &nCS     = engine.getCS<idk::Name_CS>();
    auto &iCS     = engine.getCS<idk::Icon_CS>();
    auto &tCS     = engine.getCS<idk::Transform_CS>();
    auto &objects = engine.gameObjects();
    int  obj_id   = 0; // root object

    static std::string *text = nullptr;

    ImGui::Begin("Scene Hierarchy");

        ImGui::BeginChild("split-window", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
        _tab_scene_treenode(api, obj_id);
        ImGui::EndChild();


        if (ImGui::Button(IDK_ICON_PLUS " Create"))
        {
            // m_selected_object = 1;
            engine.createGameObject("Empty");
        }

        ImGui::SameLine();

        if (ImGui::Button(IDK_ICON_TRASH " delete"))
        {
            engine.deleteGameObject(_get_selection());
            std::cout << "Deleted object " << _get_selection() << "\n";
            m_selection.pop_front();
        }

    ImGui::End();

}

