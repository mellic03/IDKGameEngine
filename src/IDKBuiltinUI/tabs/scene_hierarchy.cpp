#include "../EditorUI.hpp"
#include "EditorUI-tabs.hpp"


static void
idk_scene_treenode_drag_drop( idecs::ECS &ecs, int obj_id )
{
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        ImGui::SetDragDropPayload(
            "SCENE_HIERARCHY",
            reinterpret_cast<const void *>(&obj_id),
            sizeof(int)
        );
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
            ecs.giveChild(obj_id, child_id);
        }

        else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCRIPT_DRAG_DROP"))
        {
            std::string name = (char *)(payload->Data);
            ecs.getSystem<idk::ScriptSys>().assignScript(obj_id, name);
        }

        ImGui::EndDragDropTarget();
    }
}


static void
idk_scene_treenode_drag_drop_deparent( idecs::ECS &ecs, int obj_id )
{
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        ImGui::SetDragDropPayload(
            "SCENE_HIERARCHY",
            reinterpret_cast<const void *>(&obj_id),
            sizeof(int)
        );
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
            ecs.removeParent(child_id);
        }
        ImGui::EndDragDropTarget();
    }
}


// static void
// idk_scene_drag_drop_CS( idecs::ECS &ecs, int obj_id )
// {
//     if (ImGui::BeginDragDropTarget())
//     {
//         ImGui::BeginTooltip();
//         ImGui::TextColored({255, 0, 0, 255}, "Parent");
//         ImGui::EndTooltip();

//         if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IDK_CS_DRAG_DROP"))
//         {
//             IM_ASSERT(payload->DataSize == sizeof(int));

//             int component = *reinterpret_cast<int *>(payload->Data);
//             ecs.giveComponent(obj_id, component);
//         }
//         ImGui::EndDragDropTarget();
//     } 
// }





void
EditorUI_MD::_tab_scene_treenode( idk::EngineAPI &api, int id )
{
    if (id == -1)
    {
        return;
    }

    auto &engine  = api.getEngine();
    auto &ecs     = api.getECS();

    std::string icon = ecs.getComponent<idk::IconCmp>(id).icon;

    std::string name = ecs.getGameObjectName(id);
    std::string label = icon + std::string(" ") + name;
    int flags  = ImGuiTreeNodeFlags_DefaultOpen;
        flags |= ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanFullWidth;
        flags |= ImGuiTreeNodeFlags_SpanAllColumns;

    if (ecs.getChildren(id).size() == 0)
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (id == ecs.getSelectedGameObject())
    {
        flags |= ImGuiTreeNodeFlags_Selected;
    }


    ImGui::PushID(id);


    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    bool node_open = ImGui::TreeNodeEx(label.c_str(), flags);
    bool row_clicked = ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left);


    if (row_clicked && !ImGui::IsItemToggledOpen())
    {
        ecs.setSelectedGameObject(id);
    }

    if (node_open)
    {
        if (ImGui::BeginPopupContextWindow("Object Context"))
        {
            // auto icon = api.getEngine().getCS<idk::Icon_CS>().getIcon(id);
            std::string label = ecs.getSelectedGameObjectName();
                        label = icon + " " + label;

            ImGui::Text(label.c_str());
            ImGui::Separator();

            if (ImGui::MenuItem("Copy"))
            {
                ecs.copySelectedGameObject();
            }

            if (ImGui::MenuItem("Delete"))
            {
                ecs.deleteSelectedGameObject();
            }

            ImGui::EndPopup();
        }


        // float width = ImGui::GetContentRegionAvail().x;
        // ImGui::SameLine(width - 1);
        // ImGui::Text(ICON_FA_BARS);
    
        idk_scene_treenode_drag_drop(ecs, id);
        // idk_scene_drag_drop_CS(id, engine);

        for (int child_id: ecs.getChildren(id))
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



void
EditorUI_MD::_tab_scene_hierarchy( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();
    auto &ecs    = api.getECS();

    int  obj_id   = 0;

    int flags  = ImGuiTreeNodeFlags_DefaultOpen;
        flags |= ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanFullWidth;
        flags |= ImGuiTreeNodeFlags_SpanAllColumns;

    uint32_t table_flags = ImGuiTableFlags_Borders
                         | ImGuiTableFlags_RowBg
                         | ImGuiTableFlags_ScrollX
                         | ImGuiTableFlags_ScrollY;

    static std::string *text = nullptr;

    ImGui::Begin("Scene Hierarchy");

        if (ImGui::BeginTable("Hierarchy Table", 1, table_flags, ImVec2(0, -ImGui::GetFrameHeightWithSpacing())))
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            bool row_clicked = ImGui::IsItemClicked() && ImGui::IsMouseReleased(ImGuiMouseButton_Left);
            bool node_open = ImGui::TreeNodeEx("root", flags);

            if (row_clicked && !ImGui::IsItemToggledOpen())
            {
                ecs.setSelectedGameObject(obj_id);
            }

            if (node_open)
            {
                idk_scene_treenode_drag_drop_deparent(ecs, obj_id);

                for (int id: ecs.gameObjects())
                {
                    if (ecs.hasParent(id) == false)
                    {
                        _tab_scene_treenode(api, id);
                    }
                }

                ImGui::TreePop();
            }
            ImGui::EndTable();
        }


        if (ImGui::Button(ICON_FA_PLUS " Create"))
        {
            ecs.createGameObject("Empty");
        }

        ImGui::SameLine();

        if (ImGui::Button(ICON_FA_TRASH_CAN " delete"))
        {
            ecs.deleteSelectedGameObject();
        }

    ImGui::End();

}

