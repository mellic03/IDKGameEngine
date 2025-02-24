#include "../EditorUI.hpp"
#include <IDKECS/IDKECS.hpp>
#include "EditorUI-tabs.hpp"

#include <filesystem>
namespace fs = std::filesystem;

template <typename T>
void idk_file_ree2( idk::ECS &ecs, const std::string &filepath, int obj_id );



template <>
void
idk_file_ree2<idk::AudioEmitterCmp>( idk::ECS &ecs, const std::string &filepath, int obj_id )
{
    idk::AudioSys::assignSound(obj_id, filepath);
}


template <>
void
idk_file_ree2<idk::ModelCmp>( idk::ECS &ecs, const std::string &filepath, int obj_id )
{
    auto &msys = ecs.getSystem<idk::ModelSys>();
    msys.assignModel(obj_id, filepath.c_str());
}



void idk_file_ree( idk::ECS &ecs, const std::string &filepath, int obj_id )
{
    std::string ext = fs::path(filepath).extension().string();

    if (ext == ".wav" || ext == ".mp3")
    {
        idk_file_ree2<idk::AudioEmitterCmp>(ecs, filepath, obj_id);
    }

    else if (ext == ".idkvi")
    {
        idk_file_ree2<idk::ModelCmp>(ecs, filepath, obj_id);
    }
}




static void
idk_scene_treenode_drag_drop( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
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

        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_DRAG_DROP"))
        {
            std::string filepath(reinterpret_cast<char *>(payload->Data));
            idk_file_ree(ecs, filepath, obj_id);
        }

        else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
        {
            IM_ASSERT(payload->DataSize == sizeof(int));
            int child_id = *reinterpret_cast<int *>(payload->Data);
            ecs.giveChild(obj_id, child_id);
        }
    
        ImGui::EndDragDropTarget();
    }
}


static void
idk_scene_treenode_drag_drop_deparent( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
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
            
            int parent = ecs.getParent(child_id);
            if (parent != -1)
            {
                ecs.removeChild(parent, child_id);
            }
        }
        ImGui::EndDragDropTarget();
    }
}



void
EditorUI_MD::_tab_scene_treenode( idk::EngineAPI &api, idk::ECS &ecs, int id )
{
    if (id == -1)
    {
        return;
    }

    auto &engine = api.getEngine();

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
    
        idk_scene_treenode_drag_drop(api, ecs, id);

        for (int child_id: ecs.getChildren(id))
        {
            _tab_scene_treenode(api, ecs, child_id);
        }
    
        ImGui::TreePop();
    }

    ImGui::PopID();
}



void prefab_popup( idk::EngineAPI &api, idk::ECS &ecs )
{
    if (ImGui::BeginPopup("Instantiate Prefab"))
    {
        std::string label = "Instantiate Prefab";
        ImGui::Text(label.c_str());
        ImGui::Separator();

        for (auto &[name, callback]: ecs.getPrefabs())
        {
            if (ImGui::MenuItem(name.c_str()))
            {
                ecs.createGameObjectFromPrefab(name);
            }
        }

        ImGui::EndPopup();
    }
}


void
EditorUI_MD::_tab_scene_hierarchy( idk::EngineAPI &api, idk::ECS &ecs )
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

    ImGui::Begin("Scene Hierarchy");


    if (ImGui::BeginTable("Hierarchy Table", 1, table_flags, ImVec2(0, -ImGui::GetFrameHeightWithSpacing())))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        bool row_clicked = ImGui::IsItemClicked() && ImGui::IsMouseReleased(ImGuiMouseButton_Left);
        bool node_open = ImGui::TreeNodeEx(ecs.name.c_str(), flags);

        if (row_clicked && !ImGui::IsItemToggledOpen())
        {
            ecs.setSelectedGameObject(obj_id);
        }

        if (node_open)
        {
            idk_scene_treenode_drag_drop_deparent(api, ecs, obj_id);

            for (auto &e: ecs.getEntities())
            {
                int id = e.id;

                if (ecs.hasParent(id) == false)
                {
                    _tab_scene_treenode(api, ecs, id);
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

    // bool prefab_popup_open = true;
    if (ImGui::Button(ICON_FA_PLUS " Prefab"))
    {
        ImGui::OpenPopup("Instantiate Prefab");
        // prefab_popup_open = true;
    }

    ImGui::SameLine();

    if (ImGui::Button(ICON_FA_TRASH_CAN " delete"))
    {
        ecs.deleteSelectedGameObject();
    }


    // if (prefab_popup_open)
    {
        prefab_popup(api, ecs);
    }



    ImGui::End();

}

