#include "../EditorUI.hpp"
#include "EditorUI-tabs.hpp"

#include <filesystem>
namespace fs = std::filesystem;


// static void
// idk_scene_treenode_drag_drop( int obj_id )
// {
//     if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
//     {
//         ImGui::SetDragDropPayload(
//             "SCENE_HIERARCHY",
//             reinterpret_cast<const void *>(&obj_id),
//             sizeof(int)
//         );
//         ImGui::EndDragDropSource();
//     }

//     if (ImGui::BeginDragDropTarget())
//     {
//         ImGui::BeginTooltip();
//         ImGui::TextColored({255, 0, 0, 255}, "Parent");
//         ImGui::EndTooltip();

//         if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_DRAG_DROP"))
//         {
//             std::string filepath(reinterpret_cast<char *>(payload->Data));
//             idk_file_ree(filepath, obj_id);
//         }

//         else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
//         {
//             IM_ASSERT(payload->DataSize == sizeof(int));
//             int child_id = *reinterpret_cast<int *>(payload->Data);
//             idk::ECS2::giveChild(obj_id, child_id);
//         }
    
//         ImGui::EndDragDropTarget();
//     }
// }


// static void
// idk_scene_treenode_drag_drop_deparent( int obj_id )
// {
//     if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
//     {
//         ImGui::SetDragDropPayload(
//             "SCENE_HIERARCHY",
//             reinterpret_cast<const void *>(&obj_id),
//             sizeof(int)
//         );
//         ImGui::EndDragDropSource();
//     }

//     if (ImGui::BeginDragDropTarget())
//     {
//         ImGui::BeginTooltip();
//         ImGui::TextColored({255, 0, 0, 255}, "Parent");
//         ImGui::EndTooltip();

//         if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
//         {
//             IM_ASSERT(payload->DataSize == sizeof(int));
//             int child_id = *reinterpret_cast<int *>(payload->Data);
            
//             int parent = idk::ECS2::getParent(child_id);
//             if (parent != -1)
//             {
//                 idk::ECS2::removeChild(parent, child_id);
//             }
//         }
//         ImGui::EndDragDropTarget();
//     }
// }



// void
// EditorUI_MD::_tab_scene_treenode( idk::EngineAPI &api, int id )
// {
//     if (id == -1)
//     {
//         return;
//     }

//     auto &engine  = api.getEngine();
    

//     std::string icon = idk::ECS2::getComponent<idk::IconCmp>(id).icon;

//     std::string name = idk::ECS2::getGameObjectName(id);
//     std::string label = icon + std::string(" ") + name;
//     int flags  = ImGuiTreeNodeFlags_DefaultOpen;
//         flags |= ImGuiTreeNodeFlags_OpenOnArrow;
//         flags |= ImGuiTreeNodeFlags_SpanFullWidth;
//         flags |= ImGuiTreeNodeFlags_SpanAllColumns;

//     if (idk::ECS2::getChildren(id).size() == 0)
//     {
//         flags |= ImGuiTreeNodeFlags_Leaf;
//     }

//     if (id == idk::ECS2::getSelectedGameObject())
//     {
//         flags |= ImGuiTreeNodeFlags_Selected;
//     }


//     ImGui::PushID(id);


//     ImGui::TableNextRow();
//     ImGui::TableNextColumn();
//     bool node_open = ImGui::TreeNodeEx(label.c_str(), flags);
//     bool row_clicked = ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left);


//     if (row_clicked && !ImGui::IsItemToggledOpen())
//     {
//         idk::ECS2::setSelectedGameObject(id);
//     }

//     if (node_open)
//     {
//         if (ImGui::BeginPopupContextWindow("Object Context"))
//         {
//             std::string label = idk::ECS2::getSelectedGameObjectName();
//                         label = icon + " " + label;

//             ImGui::Text(label.c_str());
//             ImGui::Separator();

//             if (ImGui::MenuItem("Copy"))
//             {
//                 idk::ECS2::copySelectedGameObject();
//             }

//             if (ImGui::MenuItem("Delete"))
//             {
//                 idk::ECS2::deleteSelectedGameObject();
//             }

//             ImGui::EndPopup();
//         }
    
//         idk_scene_treenode_drag_drop( id);

//         for (int child_id: idk::ECS2::getChildren(id))
//         {
//             _tab_scene_treenode(api, child_id);
//         }
    
//         ImGui::TreePop();
//     }

//     ImGui::PopID();
// }



// void prefab_popup( idk::EngineAPI &api )
// {
//     if (ImGui::BeginPopup("Instantiate Prefab"))
//     {
//         std::string label = "Instantiate Prefab";
//         ImGui::Text(label.c_str());
//         ImGui::Separator();

//         for (auto &[name, callback]: idk::ECS2::getPrefabs())
//         {
//             if (ImGui::MenuItem(name.c_str()))
//             {
//                 idk::ECS2::createGameObjectFromPrefab(name);
//             }
//         }

//         ImGui::EndPopup();
//     }
// }


void
EditorUI_MD::_tab_render_hierarchy( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();
    int  obj_id  = 0;

    int flags  = ImGuiTreeNodeFlags_DefaultOpen;
        flags |= ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanFullWidth;
        flags |= ImGuiTreeNodeFlags_SpanAllColumns;

    uint32_t table_flags = ImGuiTableFlags_Borders
                         | ImGuiTableFlags_RowBg
                         | ImGuiTableFlags_ScrollX
                         | ImGuiTableFlags_ScrollY;

    static std::string *text = nullptr;

    ImGui::Begin("Render Hierarchy");


    if (ImGui::BeginTable("Hierarchy Table", 1, table_flags, ImVec2(0, -ImGui::GetFrameHeightWithSpacing())))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        bool row_clicked = ImGui::IsItemClicked() && ImGui::IsMouseReleased(ImGuiMouseButton_Left);
        bool node_open = ImGui::TreeNodeEx("root", flags);

        if (row_clicked && !ImGui::IsItemToggledOpen())
        {
            // idk::ECS2::setSelectedGameObject(obj_id);
        }

        if (node_open)
        {
            for (auto &[node_id, node]: ren.getNodeWAllocator())
            {
                if (ImGui::TreeNode(std::to_string(node.model).c_str()))
                {
                    ImGui::TreePop();
                }
            }

            ImGui::TreePop();
        }
        ImGui::EndTable();
    }


    ImGui::End();

}

