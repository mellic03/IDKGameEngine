#include "EditorUI.hpp"

#include "../../external/include/idk_icons/idk_Icons.hpp"
#include "../../external/include/idk_imgui/imgui.hpp"
#include "../IDKBuiltinUI/common/idk_imgui_extra.hpp"

#include <typeindex>


template <>
void
EditorUI_MD::drawComponent<idk::TransformCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &engine = api.getEngine();
    auto &ecs    = api.getECS();

    auto &data   = idk::TransformSys::getData(obj_id);

    ImGui::InputFloat3("Tr", &data.position[0], "%0.2f");
    ImGui::SameLine();
    if (ImGui::Button("Reset Tr"))
    {
        data.position = glm::vec3(0.0f);
    }

    ImGui::InputFloat3("Rt", &data.rotation[0], "%0.2f");
    ImGui::SameLine();
    if (ImGui::Button("Reset Rt"))
    {
        data.rotation = glm::quat();
    }

    ImGui::DragFloat3("Sc", &data.scale[0], 0.01f, 0.05f, 0.0f, "%0.2f");
    ImGui::SameLine();
    if (ImGui::Button("Reset Sc"))
    {
        data.scale = glm::vec3(1.0f);
    }

    // static bool b = false;

    // if (ImGui::RadioButton("Local", b))  b = true;
    // ImGui::SameLine();
    // if (ImGui::RadioButton("World", !b)) b = false;

}



template <>
void
EditorUI_MD::drawComponent<idk::CameraCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ecs = api.getECS();
    auto &cmp = ecs.getComponent<idk::CameraCmp>(obj_id);


    // Camera drag-drop
    // -----------------------------------------------------------------------------------------
    std::string label = "Camera " ICON_FA_DOWNLOAD;
    if (cmp.cam_id != -1)
    {
        label = std::to_string(cmp.cam_id);
    }

    ImGui::Button(label.c_str(), ImVec2(-FLT_MIN, 25));

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CAMERA_DRAG_DROP"))
        {
            IM_ASSERT(payload->DataSize == sizeof(int));
        
            cmp.obj_id = obj_id;
            cmp.cam_id = *reinterpret_cast<int *>(payload->Data);
        }
        ImGui::EndDragDropTarget();
    }
    // -----------------------------------------------------------------------------------------

}


template <>
void
EditorUI_MD::drawComponent<idk::PhysicsMotionCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ecs = api.getECS();
    auto &cmp = ecs.getComponent<idk::PhysicsMotionCmp>(obj_id);

    ImGui::SeparatorText("Linear Velocity");
    ImGui::InputFloat("Drag ##1",      &cmp.lin_drag);
    ImGui::InputFloat3("Magnitude ##1", &cmp.lin_velocity[0], "%1.f");

    ImGui::SeparatorText("Angular Velocity");
    ImGui::InputFloat("Drag ##2",      &cmp.ang_drag);
    ImGui::InputFloat("Magnitude ##2", &cmp.ang_velocity);
    ImGui::InputFloat3("Axis ##2",     &cmp.ang_axis[0], "%1.f");

}



template <>
void
EditorUI_MD::drawComponent<idk::BoxColliderCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ecs = api.getECS();
    auto &cmp = ecs.getComponent<idk::BoxColliderCmp>(obj_id);

    ImGui::Checkbox("Visualize", &cmp.visualize);
}


template <>
void
EditorUI_MD::drawComponent<idk::SphereColliderCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ecs = api.getECS();
    auto &cmp = ecs.getComponent<idk::SphereColliderCmp>(obj_id);

    ImGui::Checkbox("Visualize", &cmp.visualize);
}


template <>
void
EditorUI_MD::drawComponent<idk::ModelCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ecs = api.getECS();
    auto &cmp = ecs.getComponent<idk::ModelCmp>(obj_id);


    // Model drag-drop
    // -----------------------------------------------------------------------------------------
    std::string label = "Model " ICON_FA_DOWNLOAD;
    if (cmp.model_id != -1)
    {
        label = api.getRenderer().modelSystem().getModel(cmp.model_id).name;
    }

    ImGui::Button(label.c_str(), ImVec2(-FLT_MIN, 25));

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MODEL_DRAG_DROP"))
        {
            IM_ASSERT(payload->DataSize == sizeof(int));

            int model_id = *reinterpret_cast<int *>(payload->Data);
            idk::ModelSys::assignModel(obj_id, model_id);
        }
        ImGui::EndDragDropTarget();
    }
    // -----------------------------------------------------------------------------------------

    ImGui::Checkbox("Visible",     &cmp.visible);
    ImGui::Checkbox("Shadow",      &cmp.shadowcast);
    // ImGui::Checkbox("Environment", &cmp.environment);
}



template <>
void
EditorUI_MD::drawComponent<idk::IconCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &data = api.getECS().getComponent<idk::IconCmp>(obj_id);
    std::string icon = std::string(data.icon);

    size_t NUM_COLS = size_t(ImGui::GetWindowWidth() / ImGui::GetFontSize()) / 2;
           NUM_COLS = (NUM_COLS <= 0) ? 1 : NUM_COLS;

    if (ImGui::BeginTable("Icons", NUM_COLS))
    {
        ImGui::TableNextRow();
        
        for (size_t i=0; i<IDK_ICONS.size(); i++)
        {
            if (i % NUM_COLS == 0)
            {
                ImGui::TableNextRow;
            }

            ImGui::TableNextColumn();

            bool selected = icon == IDK_ICONS[i];

            if (ImGui::Selectable(IDK_ICONS[i], &selected))
            {
                std::strcpy(data.icon, IDK_ICONS[i]);
                icon = IDK_ICONS[i];
            }
        }
    
        ImGui::EndTable();
    }
}


template <>
void
EditorUI_MD::drawComponent<idk::ScriptCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &engine = api.getEngine();
    auto &ecs    = api.getECS();
    auto &cmp    = ecs.getComponent<idk::ScriptCmp>(obj_id);
    float width  = ImGui::GetWindowWidth() / 2.0f;


    // Script drag-drop
    // -----------------------------------------------------------------------------------------
    std::string label = "Script " ICON_FA_DOWNLOAD;
    if (std::string(cmp.filepath) != "")
    {
        label = std::string(cmp.filepath);
    }

    ImGui::Button(label.c_str(), ImVec2(-FLT_MIN, 25));

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCRIPT_DRAG_DROP"))
        {
            std::strcpy(cmp.filepath, (char *)(payload->Data));
            cmp.nparams = idk::ScriptCmp_getNumParams(cmp);
        }
        ImGui::EndDragDropTarget();
    }
    // -----------------------------------------------------------------------------------------

    if (cmp.nparams == 1)
    {
        width *= 2.0f;
    }

    if (cmp.nparams >= 1)
    {
        // Subject drag-drop
        // -----------------------------------------------------------------------------------------
        label = "Subject " ICON_FA_DOWNLOAD;
        if (cmp.subject_id != -1)
        {
            label = ecs.getGameObjectName(cmp.subject_id);
        }

        ImGui::ButtonEx(label.c_str(), ImVec2(width, 50), ImGuiButtonFlags_None);

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
            {
                IM_ASSERT(payload->DataSize == sizeof(int));
                cmp.subject_id = *reinterpret_cast<int *>(payload->Data);
            }
            ImGui::EndDragDropTarget();
        }
        // -----------------------------------------------------------------------------------------
    }


    if (cmp.nparams == 2)
    {
        ImGui::SameLine();

        // Target drag-drop
        // -----------------------------------------------------------------------------------------
        label = "Target " ICON_FA_DOWNLOAD;
        if (cmp.target_id != -1)
        {
            label = ecs.getGameObjectName(cmp.target_id);
        }

        ImGui::ButtonEx(label.c_str(), ImVec2(width, 50), ImGuiButtonFlags_None);

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
            {
                IM_ASSERT(payload->DataSize == sizeof(int));
                cmp.target_id = *reinterpret_cast<int *>(payload->Data);
            }
            ImGui::EndDragDropTarget();
        }
        // -----------------------------------------------------------------------------------------
    }



    // Target drag-drop
    // -----------------------------------------------------------------------------------------
    label = "Depends on " ICON_FA_DOWNLOAD;
    if (cmp.dependency != -1)
    {
        label = std::string("Depends on ") + ecs.getGameObjectName(cmp.target_id);
    }

    ImGui::ButtonEx(label.c_str(), ImVec2(width, 50), ImGuiButtonFlags_None);

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
        {
            IM_ASSERT(payload->DataSize == sizeof(int));
            cmp.dependency = *reinterpret_cast<int *>(payload->Data);
        }
        ImGui::EndDragDropTarget();
    }
    // -----------------------------------------------------------------------------------------


    // ImGui::PopStyleColor();
}
