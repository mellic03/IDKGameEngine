#include "EditorUI.hpp"

#include "../../external/include/idk_icons/idk_Icons.hpp"
#include "../../external/include/idk_imgui/imgui.hpp"
#include "../IDKBuiltinUI/common/idk_imgui_extra.hpp"

#include <filesystem>
namespace fs = std::filesystem;


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
        data.rotation = glm::quat(glm::vec3(0.0f));
    }

    ImGui::DragFloat("Pitch", &data.pitch, 0.025f);
    ImGui::DragFloat("Yaw",   &data.yaw,   0.025f);
    ImGui::DragFloat("Roll",  &data.roll,  0.025f);


    ImGui::InputFloat("Sc", &data.scale, 0.1f);
    ImGui::SameLine();
    if (ImGui::Button("Reset Sc"))
    {
        data.scale = 1.0f;
    }

    ImGui::InputFloat3("Sc3", &data.scale3[0], "%0.2f");
    ImGui::SameLine();
    if (ImGui::Button("Reset Sc3"))
    {
        data.scale3 = glm::vec3(1.0f);
    }



    // Surface up/right/front
    ImGui::InputFloat3("Surface Up", &data.up[0], "%0.2f");
    ImGui::SameLine();
    if (ImGui::Button("Reset Surface Up"))
    {
        data.up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    ImGui::InputFloat3("Surface Right", &data.right[0], "%0.2f");
    ImGui::SameLine();
    if (ImGui::Button("Reset Surface Right"))
    {
        data.right = glm::vec3(1.0f, 0.0f, 0.0f);
    }

    ImGui::InputFloat3("Surface Front", &data.front[0], "%0.2f");
    ImGui::SameLine();
    if (ImGui::Button("Reset Surface Front"))
    {
        data.front = glm::vec3(0.0f, 0.0f, -1.0f);
    }


}



template <>
void
EditorUI_MD::drawComponent<idk::CameraCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ren = api.getRenderer();
    auto &ecs = api.getECS();
    auto &cmp = ecs.getComponent<idk::CameraCmp>(obj_id);

    ImGui::DragFloat("Bloom", &cmp.bloom, 0.01f, 0.0f, 1.0f);


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
EditorUI_MD::drawComponent<idk::SunCmp>( idk::EngineAPI &api, int obj_id )
{

}


template <>
void
EditorUI_MD::drawComponent<idk::PlanetCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ren = api.getRenderer();
    auto &ecs = api.getECS();
    auto &cmp = ecs.getComponent<idk::PlanetCmp>(obj_id);

    ImGui::DragFloat("Gravity", &cmp.gravity, 0.01f, 0.1f, 10.0f);
    ImGui::DragFloat("SOI",     &cmp.SOI,     0.1f, 128.0f);

    ImGui::Separator();

    ImGui::DragFloat3("Orbital Origin", &cmp.orbital_origin[0], 1.0f, 100.0f, 10000.0f);
    ImGui::DragFloat3("Orbital Radii",  &cmp.orbital_radii[0], 1.0f, 100.0f, 10000.0f);
    ImGui::DragFloat3("Orbital Speed",  &cmp.orbital_speed[0], 0.01f, -1.0f, 1.0f);

}


template <>
void
EditorUI_MD::drawComponent<idk::PlanetActorCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ecs = api.getECS();
    auto &cmp = ecs.getComponent<idk::PlanetActorCmp>(obj_id);

    ImGui::Checkbox("Enabled", &cmp.enabled);

}


template <>
void
EditorUI_MD::drawComponent<idk::AtmosphereCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ren = api.getRenderer();
    auto &ecs = api.getECS();
    auto &cmp = ecs.getComponent<idk::AtmosphereCmp>(obj_id);

    if (cmp.atmosphere_id == -1)
    {
        return;
    }

    ImGui::DragFloat("Sea Level", &cmp.sealevel, 10.0f, 200.0f);

    ImGui::InputFloat("Lambda R",        &cmp.wavelengths[0]);
    ImGui::InputFloat("Lambda G",        &cmp.wavelengths[1]);
    ImGui::InputFloat("Lambda B",        &cmp.wavelengths[2]);
    ImGui::DragFloat("Radius",           &cmp.radius,            0.1f,  10.0f);
    ImGui::DragFloat("Density Falloff",  &cmp.density_falloff,   0.01f, 0.5f);
    ImGui::DragFloat("Scatter Strength", &cmp.scatter_strength,  0.01f, 0.1f);
    ImGui::DragFloat("Atmosphere Scale", &cmp.atmosphere_scale,  0.01f, 1.1f);

}



template <>
void
EditorUI_MD::drawComponent<idk::SpotlightCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ren = api.getRenderer();
    auto &ecs = api.getECS();
    auto &cmp = ecs.getComponent<idk::SpotlightCmp>(obj_id);

    if (cmp.light_id == -1)
    {
        return;
    }

    ImGui::ColorEdit3("Diffuse",      &cmp.diffuse[0]);
    ImGui::DragFloat("Specular",      &cmp.diffuse[3], 0.05f, 0.0f, 1.0f);
    ImGui::DragFloat("Radius",        &cmp.radius,     0.01f, 0.5f, 64.0f);
    ImGui::InputFloat("Inner cutoff", &cmp.angle[0]);
    ImGui::InputFloat("Outer cutoff", &cmp.angle[1]);

}


template <>
void
EditorUI_MD::drawComponent<idk::PointlightCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ren = api.getRenderer();
    auto &ecs = api.getECS();
    auto &cmp = ecs.getComponent<idk::PointlightCmp>(obj_id);

    if (cmp.light_id == -1)
    {
        return;
    }

    ImGui::ColorEdit3("Diffuse",      &cmp.diffuse[0]);
    ImGui::DragFloat("Specular",      &cmp.diffuse[3], 0.05f, 0.0f, 1.0f);
    ImGui::DragFloat("Radius",        &cmp.radius,     0.01f, 0.5f, 64.0f);

}




template <>
void
EditorUI_MD::drawComponent<idk::ModelCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ecs = api.getECS();
    auto &cmp = ecs.getComponent<idk::ModelCmp>(obj_id);


    ImGui::Text("Model ID: %d", cmp.model_id);
    ImGui::Separator();


    // Model drag-drop
    // -----------------------------------------------------------------------------------------
    std::string label = "Model " ICON_FA_DOWNLOAD;
    if (cmp.model_id != -1)
    {
        label = fs::path(cmp.filepath).filename();
    }

    ImGui::Button(label.c_str(), ImVec2(-FLT_MIN, 25));

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MODEL_DRAG_DROP"))
        {
            IM_ASSERT(payload->DataSize == sizeof(int));

            int model_id = *reinterpret_cast<int *>(payload->Data);
            // idk::ModelSys::assignModel(obj_id, model_id);
        }
        ImGui::EndDragDropTarget();
    }
    // -----------------------------------------------------------------------------------------

    ImGui::Checkbox("Visible",     &cmp.visible);
    ImGui::Checkbox("Shadow",      &cmp.shadowcast);
    ImGui::Checkbox("Viewspace",   &cmp.viewspace);
}



template <>
void
EditorUI_MD::drawComponent<idk::IconCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &data = api.getECS().getComponent<idk::IconCmp>(obj_id);
    std::string icon = std::string(data.icon);

    size_t NUM_COLS = size_t(ImGui::GetWindowWidth() / ImGui::GetFontSize()) / 2;
           NUM_COLS = (NUM_COLS <= 0) ? 1 : NUM_COLS;


    ImGui::Text("Object ID: %d", obj_id);
    ImGui::Separator();

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
                data.icon = IDK_ICONS[i];
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

    ImGui::Checkbox("Enable", &cmp.enabled);

    // Script drag-drop
    // -----------------------------------------------------------------------------------------
    std::string label = "Script " ICON_FA_DOWNLOAD;
    std::string &filepath = cmp.filepath;

    if (filepath != "")
    {
        std::string filename = fs::path(filepath).stem();
        label = filename;
    }

    ImGui::Button(label.c_str(), ImVec2(-FLT_MIN, 25));

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCRIPT_DRAG_DROP"))
        {
            cmp.filepath = std::string((char *)(payload->Data));
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
            if (ecs.gameObjectExists(cmp.subject_id))
            {
                label = ecs.getGameObjectName(cmp.subject_id);
            }

            else
            {
                cmp.subject_id = -1;
            }
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




template <>
void
EditorUI_MD::drawComponent<idk::AudioEmitterCmp>( idk::EngineAPI &api, int obj_id )
{
    std::string label = api.getECS().getComponent<idk::AudioEmitterCmp>(obj_id).filepath;

    if (label == "")
    {
        label += "Audio " ICON_FA_DOWNLOAD; 
    }

    else
    {
        label = fs::path(label).filename();
    }


    ImGui::Button(label.c_str(), ImVec2(50, 50));

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_DRAG_DROP"))
        {
            std::string filepath(reinterpret_cast<char *>(payload->Data));
            idk::AudioSys::assignSound(obj_id, filepath);
        }
        ImGui::EndDragDropTarget();
    }
}


template <>
void
EditorUI_MD::drawComponent<idk::AudioListenerCmp>( idk::EngineAPI &api, int obj_id )
{

}


template <>
void
EditorUI_MD::drawComponent<idk::PhysXMeshRigidStaticCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ecs = api.getECS();
    ImGui::Checkbox("Enable", &ecs.getComponent<idk::PhysXMeshRigidStaticCmp>(obj_id).enabled);
}


template <>
void
EditorUI_MD::drawComponent<idk::PhysXSphereRigidDynamicCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ecs = api.getECS();
    auto &cmp = ecs.getComponent<idk::PhysXSphereRigidDynamicCmp>(obj_id);

    ImGui::Checkbox("Enable", &cmp.enabled);

    if (ImGui::Button("Force Up/Left"))
    {
        cmp.body->addForce(physx::PxVec3(20.0, 20.0, 20.0));
    }

}


template <>
void
EditorUI_MD::drawComponent<idk::PhysXKinematicControllerCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &ecs = api.getECS();
    auto &cmp = ecs.getComponent<idk::PhysXKinematicControllerCmp>(obj_id);

    physx::PxController *CCT = cmp.controller;

    float offset = CCT->getContactOffset();
    ImGui::DragFloat("Contact offset", &offset, 0.01f);
    CCT->setContactOffset(offset);

    ImGui::Checkbox("Enable", &cmp.enabled);
}



template <>
void
EditorUI_MD::drawComponent<idk::ProgressionEventCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &engine = api.getEngine();
    auto &ecs    = api.getECS();
    auto &cmp    = ecs.getComponent<idk::ProgressionEventCmp>(obj_id);
    float width  = ImGui::GetWindowWidth() / 2.0f;

    int value = int(cmp.prereq_progress);
    ImGui::InputInt("Prerequisite progress", &value);
    cmp.prereq_progress = uint32_t(value);


    // Script drag-drop
    // -----------------------------------------------------------------------------------------
    std::string label = "Script " ICON_FA_DOWNLOAD;

    if (cmp.script_id != -1)
    {
        label = ecs.getComponent<idk::ScriptCmp>(cmp.script_id).filepath;
    }

    ImGui::Button(label.c_str(), ImVec2(-FLT_MIN, 25));

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
        {
            IM_ASSERT(payload->DataSize == sizeof(int));
            cmp.script_id = *reinterpret_cast<int *>(payload->Data);
        }
        ImGui::EndDragDropTarget();
    }
    // -----------------------------------------------------------------------------------------


    // State drag-drop
    // -----------------------------------------------------------------------------------------
    label = "State " ICON_FA_DOWNLOAD;
    if (cmp.state_id != -1)
    {
        label = ecs.getGameObjectName(cmp.state_id);
    }

    ImGui::ButtonEx(label.c_str(), ImVec2(width, 50), ImGuiButtonFlags_None);

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
        {
            IM_ASSERT(payload->DataSize == sizeof(int));
            cmp.state_id = *reinterpret_cast<int *>(payload->Data);
        }
        ImGui::EndDragDropTarget();
    }
    // -----------------------------------------------------------------------------------------

}


template <>
void
EditorUI_MD::drawComponent<idk::ProgressionStateCmp>( idk::EngineAPI &api, int obj_id )
{
    auto &cmp = api.getECS().getComponent<idk::ProgressionStateCmp>(obj_id);
    int value = int(cmp.progress);
    ImGui::InputInt("Progression State", &value);
    cmp.progress = uint32_t(value);

    for (int i=0; i<16; i++)
    {
        value = int(cmp.states[i]);
        std::string label = "State " + std::to_string(i);
        ImGui::InputInt(label.c_str(), &value);
        cmp.states[i] = uint32_t(value);
    }

}





template <>
void
EditorUI_MD::drawComponent<idk::PlayerControllerCmp>( idk::EngineAPI &api, int obj_id )
{

}





#define ECS_COMPONENT_CALLBACK(component_type) \
{ \
    ecs.getComponentArray<component_type>().setDrawCallback( \
        EditorUI_MD::drawComponent<component_type> \
    ); \
}


void
EditorUI_MD::registerDrawComponents( idk::EngineAPI &api )
{
    auto &ecs = api.getECS();

    ECS_COMPONENT_CALLBACK(idk::IconCmp);
    ECS_COMPONENT_CALLBACK(idk::TransformCmp);
    ECS_COMPONENT_CALLBACK(idk::ModelCmp);
    ECS_COMPONENT_CALLBACK(idk::ScriptCmp);
    ECS_COMPONENT_CALLBACK(idk::CameraCmp);

    ECS_COMPONENT_CALLBACK(idk::SunCmp);
    ECS_COMPONENT_CALLBACK(idk::PlanetCmp);
    ECS_COMPONENT_CALLBACK(idk::PlanetActorCmp);
    ECS_COMPONENT_CALLBACK(idk::AtmosphereCmp);

    ECS_COMPONENT_CALLBACK(idk::PointlightCmp);
    ECS_COMPONENT_CALLBACK(idk::SpotlightCmp);

    ECS_COMPONENT_CALLBACK(idk::AudioEmitterCmp);
    ECS_COMPONENT_CALLBACK(idk::PhysXMeshRigidStaticCmp);
    ECS_COMPONENT_CALLBACK(idk::PhysXSphereRigidDynamicCmp);
    ECS_COMPONENT_CALLBACK(idk::PhysXKinematicControllerCmp);

    ECS_COMPONENT_CALLBACK(idk::ProgressionEventCmp);
    ECS_COMPONENT_CALLBACK(idk::ProgressionStateCmp);

    ECS_COMPONENT_CALLBACK(idk::PlayerControllerCmp);

}


