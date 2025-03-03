#include "drawcomponent.hpp"
#include <IDKGraphics/terrain/desc.hpp>
#include <IDKGraphics/idk_render_settings.hpp>
#include "common/idk_imgui_settingsfield.hpp"



template <>
void
EditorUI_MD::drawComponent<idk::TransformCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &tsys = ecs.getSystem<idk::TransformSys>();
    auto &pcmp = tsys.getTransformCmp(obj_id);
    auto &cmp  = tsys.getTransform(obj_id);

    ImGui::InputFloat3("Tr", &cmp.position[0], "%0.2f");
    ImGui::SameLine();
    if (ImGui::Button("Reset Tr"))
    {
        cmp.position = glm::vec3(0.0f);
    }

    ImGui::InputFloat4("Rt", &cmp.rotation[0], "%0.2f");
    ImGui::SameLine();
    if (ImGui::Button("Reset Rt"))
    {
        cmp.rotation = glm::quat(glm::vec3(0.0f));
    }


    ImGui::DragFloat("Pitch", &pcmp.pitch, 0.025f);
    ImGui::DragFloat("Yaw",   &pcmp.yaw,   0.025f);
    ImGui::DragFloat("Roll",  &pcmp.roll,  0.025f);


    ImGui::InputFloat4("Scale", &cmp.scale[0], "%0.2f");
    ImGui::SameLine();
    if (ImGui::Button("Reset Scale"))
    {
        cmp.scale = glm::vec4(1.0f);
    }

}




static void
drawNFThing( const std::string &name, idk::TerrainRenderer::NoiseFactor &NF )
{
    ImGui::SeparatorText(name.c_str());

    std::string labels[4] = { "Amp", "Wav", "Warp", "Octaves" };

    for (int i=0; i<4; i++)
    {
        labels[i] = labels[i] + "##" + name;
    }

    ImGui::DragFloat(labels[0].c_str(),  &NF.amp,     0.0001f, -1.0f, +1.0f);
    ImGui::DragFloat(labels[1].c_str(),  &NF.wav,     0.0001f,  1.0f, +8.0f);
    ImGui::DragFloat(labels[2].c_str(),  &NF.warp,    0.01f,  0.0f, +8.0f);
    ImGui::DragFloat(labels[3].c_str(),  &NF.octaves, 0.01f,  0.0f, +8.0f);
}



template <>
void
EditorUI_MD::drawComponent<idk::TerrainCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<idk::TerrainCmp>(obj_id);
    auto &desc = idk::TerrainRenderer::getTerrainDesc();

    glm::vec4 &scale = desc.scale;
    glm::vec4 &aa    = desc.slope_blend;
    glm::vec4 &bb    = desc.height_blend;
    glm::vec4 &tex   = desc.texscale[0];

    if (ImGui::BeginTabBar("Terrain", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("Terrain"))
        {
            static bool generate = true;
            static bool wireframe = false;
            ImGui::Checkbox("Generate", &generate);
            ImGui::Checkbox("Wireframe", &wireframe);
            if (generate)
            {
                idk::TerrainRenderer::generateTerrain();
            }
            idk::TerrainRenderer::setTerrainWireframe(wireframe);

            ImGui::Spacing();
            ImGui::DragFloat("Clip scale", &(desc.clipmap_size[0]), 1.0f, 8.0f, 128.0f);
            ImGui::DragFloat("No. clips",  &(desc.clipmap_size[1]), 1.0f, 1.0f, 6.0f);

            ImGui::DragFloat("Height scale", &(scale[1]), 0.05f);
            ImGui::DragFloat("Width scale",  &(scale[0]), 0.05f);
            ImGui::DragFloat("Gradient k",   &(scale[2]), 0.05f, -1.0f, +8.0f);
            ImGui::Spacing();

            ImGui::DragFloat3("Origin", &(desc.origin[0]), 0.01f);
            ImGui::Spacing();

            ImGui::DragFloat("Min height", &(desc.clamp_bounds[0]), 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Max height", &(desc.clamp_bounds[1]), 0.01f, 0.0f, 1.0f);
            ImGui::Spacing();

            drawNFThing("Perlin",  desc.perlin);
            drawNFThing("Vein",    desc.vein);
            drawNFThing("Voronoi", desc.voronoi);
            drawNFThing("Domain Warping", desc.domainwarp);
            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::DragFloat("slope min", &(aa[0]), 0.001f, 0.0f, 1.0f);
            ImGui::DragFloat("slope max", &(aa[1]), 0.001f, 0.0f, 1.0f);
            ImGui::Spacing();

            ImGui::DragFloat("height min", &(bb[0]), 0.001f, 0.0f, 1.0f);
            ImGui::DragFloat("height max", &(bb[1]), 0.001f, 0.0f, 1.0f);
            ImGui::Spacing();

            ImGui::DragFloat("texscale 0", &(tex[0]), 0.01f, 0.0f, 2.0f);
            ImGui::DragFloat("texscale 1", &(tex[1]), 0.01f, 0.0f, 2.0f);
            ImGui::DragFloat("texscale 2", &(tex[2]), 0.01f, 0.0f, 2.0f);
            ImGui::DragFloat("texscale 3", &(tex[3]), 0.01f, 0.0f, 2.0f);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Water"))
        {
            static bool enabled   = true;
            static bool wireframe = false;
            ImGui::Checkbox("Enable", &enabled);
            ImGui::Checkbox("Wireframe", &wireframe);
            idk::TerrainRenderer::setWaterActive(enabled);
            idk::TerrainRenderer::setWaterWireframe(wireframe);
            ImGui::Spacing();

            drawNFThing("Water", desc.water);

            ImGui::Spacing();
            ImGui::ColorEdit4("shallow color ##water", &(desc.water_color[0][0]));
            ImGui::ColorEdit4("deep color    ##water", &(desc.water_color[1][0]));
            ImGui::ColorEdit4("color 3       ##water", &(desc.water_color[2][0]));
            ImGui::ColorEdit4("PBR           ##water", &(desc.water_color[3][0]));

            ImGui::Spacing();
            ImGui::DragFloat("tscale  ##water", &(desc.water_scale[2]), 0.01f,   0.0f, +8.0f);
            ImGui::DragFloat("wscale  ##water", &(desc.water_scale[3]), 1.0f);

            ImGui::Spacing();
            ImGui::DragFloat("xscale  ##water", &(desc.water_scale[0]), 0.002f, -2.0f, +2.0f);
            ImGui::DragFloat("yscale  ##water", &(desc.water_scale[1]), 0.002f,  0.0f, +1.0f);

            ImGui::DragFloat("water level ##water", &desc.water_pos.y, 0.001f, 0.0f, 1.0f);
            ImGui::Spacing();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Foliage"))
        {
            if (ImGui::Button("Generate grass"))
            {
                idk::TerrainRenderer::generateGrass();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }


    // static bool gen_terrain = true;
    // static bool wf_terrain  = false;
    // static bool wf_water    = false;

    // ImGui::Checkbox("Generate terrain",  &gen_terrain);
    // ImGui::Checkbox("Terrain wireframe", &wf_terrain);
    // ImGui::Checkbox("Water wireframe",   &wf_water);

    // if (gen_terrain)
    // {
    //     idk::TerrainRenderer::generateTerrain();
    // }

    // idk::TerrainRenderer::setTerrainWireframe(wf_terrain);
    // idk::TerrainRenderer::setWaterWireframe(wf_water);

}



void drag_drop_thing( idk::ECS &ecs, std::string label, const std::string &payloadname, int &obj_id )
{
    label = label + " " + std::string(ICON_FA_DOWNLOAD);

    if (obj_id != -1)
    {
        if (ecs.gameObjectExists(obj_id))
        {
            label = ecs.getGameObjectName(obj_id);
        }

        else
        {
            obj_id = -1;
        }
    }

    ImGui::ButtonEx(label.c_str(), ImVec2(250, 50), ImGuiButtonFlags_None);

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
        {
            IM_ASSERT(payload->DataSize == sizeof(int));
            obj_id = *reinterpret_cast<int *>(payload->Data);
        }
        ImGui::EndDragDropTarget();
    }
}





template <>
void
EditorUI_MD::drawComponent<idk::AnchorCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &engine = api.getEngine();
    auto &cmp    = ecs.getComponent<idk::AnchorCmp>(obj_id);

    // drag_drop_thing("Anchor Distance", "SCENE_HIERARCHY", cmp.anchor_ids[0]);
}


template <>
void
EditorUI_MD::drawComponent<idk::ParticleCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &engine = api.getEngine();
    auto &cmp    = ecs.getComponent<idk::ParticleCmp>(obj_id);
    // auto &desc   = cmp.desc;

    drag_drop_thing(ecs, "Source Object", "SCENE_HIERARCHY", cmp.src_id);

    // ImGui::InputInt("Particles", &desc.count);
    // ImGui::InputFloat("Duration", &desc.duration);
    // ImGui::InputFloat("Scale", &desc.scale);

    // ImGui::Separator();

    // ImGui::InputFloat3("Velocity", &(desc.velocity[0]));
    // ImGui::InputFloat3("Velocity bias", &(desc.velocity_bias[0]));
    // ImGui::InputFloat3("Velocity randomness", &(desc.velocity_randomness[0]));

}



template <>
void
EditorUI_MD::drawComponent<idk::SmoothFollowCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &engine = api.getEngine();
    auto &cmp    = ecs.getComponent<idk::SmoothFollowCmp>(obj_id);

    drag_drop_thing(ecs, "Anchor ID", "SCENE_HIERARCHY", cmp.anchor_id);
    ImGui::DragFloat("Speed", &cmp.speed, 0.05f, 1.0f, 0.01f);

}



template <>
void
EditorUI_MD::drawComponent<idk::IKCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &engine = api.getEngine();
    auto &cmp    = ecs.getComponent<idk::IKCmp>(obj_id);

    drag_drop_thing(ecs, "Pole Target", "SCENE_HIERARCHY", cmp.pole_target);
}



template <>
void
EditorUI_MD::drawComponent<idk::LookTowardCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &engine = api.getEngine();
    auto &cmp    = ecs.getComponent<idk::LookTowardCmp>(obj_id);

    drag_drop_thing(ecs, "Target", "SCENE_HIERARCHY", cmp.target_id);
}



template <>
void
EditorUI_MD::drawComponent<idk::RotateCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &engine = api.getEngine();
    auto &cmp    = ecs.getComponent<idk::RotateCmp>(obj_id);

    ImGui::InputFloat("Magnitude", &(cmp.magnitude));
    ImGui::DragFloat3("Axis", &(cmp.axis[0]), 0.05f, -1.0f, +1.0f);
}


template <>
void
EditorUI_MD::drawComponent<idk::CameraCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &ren = api.getRenderer();
    auto &cmp = ecs.getComponent<idk::CameraCmp>(obj_id);

    ImGui::InputFloat("FOV",  &cmp.camera.fov);
    ImGui::InputFloat("Exposure", &cmp.camera.exposure);
    ImGui::DragFloat("Bloom", &cmp.camera.bloom, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("near",  &cmp.camera.near,  0.001f, 0.01f, 2.0f);
    ImGui::DragFloat("far",   &cmp.camera.far,   1.0f, 128.0f, 2048.0f);
    // ImGui::DragFloat("FOV",   &cmp.camera.fov,   0.1f, 60.0f, 120.0f);


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



// template <>
// void
// EditorUI_MD::drawComponent<idk::AtmosphereCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
// {
//     auto &ren = api.getRenderer();
//     auto &cmp = ecs.getComponent<idk::AtmosphereCmp>(obj_id);

//     if (cmp.atmosphere_id == -1)
//     {
//         return;
//     }

//     ImGui::DragFloat("Sea Level", &cmp.sealevel, 10.0f, 200.0f);

//     ImGui::InputFloat("Lambda R",        &cmp.wavelengths[0]);
//     ImGui::InputFloat("Lambda G",        &cmp.wavelengths[1]);
//     ImGui::InputFloat("Lambda B",        &cmp.wavelengths[2]);
//     ImGui::DragFloat("Radius",           &cmp.radius,            0.1f,  10.0f);
//     ImGui::DragFloat("Density Falloff",  &cmp.density_falloff,   0.01f, 0.5f);
//     ImGui::DragFloat("Scatter Strength", &cmp.scatter_strength,  0.01f, 0.1f);
//     ImGui::DragFloat("Atmosphere Scale", &cmp.atmosphere_scale,  0.01f, 1.1f);

// }



template <>
void
EditorUI_MD::drawComponent<idk::SpotlightCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &ren = api.getRenderer();
    auto &cmp   = ecs.getComponent<idk::SpotlightCmp>(obj_id);
    auto &light = cmp.light;

    ImGui::ColorEdit4("Diffuse",      &light.diffuse[0]);
    ImGui::DragFloat("Radius",        &light.radius, 0.01f, 0.5f, 64.0f);
    ImGui::InputFloat("Inner cutoff", &light.angle[0]);
    ImGui::InputFloat("Outer cutoff", &light.angle[1]);

}



template <>
void
EditorUI_MD::drawComponent<idk::DirlightCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &ren = api.getRenderer();
    auto &cmp = ecs.getComponent<idk::DirlightCmp>(obj_id);

    if (cmp.light_id == -1)
    {
        return;
    }

    ImGui::ColorEdit4("Diffuse",  &cmp.light.diffuse[0]);
    ImGui::ColorEdit4("Ambient",  &cmp.light.ambient[0]);
    ImGui::DragFloat4("Cascades", &cmp.light.cascades[0]);
    ImGui::DragFloat4("xyzmult",  &cmp.light.cascade_zmult[0]);

}



template <>
void
EditorUI_MD::drawComponent<idk::PointlightCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &ren = api.getRenderer();
    auto &cmp = ecs.getComponent<idk::PointlightCmp>(obj_id);
    auto &light = cmp.light;

    ImGui::ColorEdit3("Diffuse",      &light.diffuse[0]);
    ImGui::DragFloat("Specular",      &light.diffuse[3], 0.05f, 0.0f, 1.0f);
    ImGui::DragFloat("Radius",        &light.radius,     0.01f, 0.5f, 64.0f);
}




template <>
void
EditorUI_MD::drawComponent<idk::ModelCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &msys = ecs.getSystem<idk::ModelSys>();
    auto &cmp  = ecs.getComponent<idk::ModelCmp>(obj_id);

    ImGui::Text("Model ID: %d", cmp.model_id);
    ImGui::Separator();


    // Model drag-drop
    // -----------------------------------------------------------------------------------------
    {
        std::string label = "Model " ICON_FA_DOWNLOAD;
        if (cmp.model_id != -1)
        {
            label = fs::path(cmp.filepath).filename();
        }

        ImGui::Button(label.c_str(), ImVec2(-FLT_MIN, 25));

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_DRAG_DROP"))
            {
                std::string filepath(reinterpret_cast<char *>(payload->Data));
            
                if (fs::path(filepath).extension().string() == ".idkvi")
                {
                    msys.assignModel(obj_id, filepath.c_str());
                }
            }
            ImGui::EndDragDropTarget();
        }
    }

    {
        std::string label = "LOD 1" ICON_FA_DOWNLOAD;
        ImGui::Button(label.c_str(), ImVec2(-FLT_MIN, 25));

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_DRAG_DROP"))
            {
                std::string filepath(reinterpret_cast<char *>(payload->Data));
            
                if (fs::path(filepath).extension().string() == ".idkvi")
                {
                    msys.assignModelLOD(obj_id, 1, filepath.c_str());
                }
            }
            ImGui::EndDragDropTarget();
        }
    }

    {
        std::string label = "LOD 2" ICON_FA_DOWNLOAD;
        ImGui::Button(label.c_str(), ImVec2(-FLT_MIN, 25));

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_DRAG_DROP"))
            {
                std::string filepath(reinterpret_cast<char *>(payload->Data));
            
                if (fs::path(filepath).extension().string() == ".idkvi")
                {
                    msys.assignModelLOD(obj_id, 2, filepath.c_str());
                }
            }
            ImGui::EndDragDropTarget();
        }
    }
    // -----------------------------------------------------------------------------------------

    ImGui::Checkbox("Visible",      &cmp.visible);
    ImGui::Checkbox("Shadow",       &cmp.shadowcast);
    ImGui::Checkbox("GI",           &cmp.environment);
    ImGui::Checkbox("Alpha cutoff", &cmp.alpha_cutoff);
    ImGui::Checkbox("Viewspace",    &cmp.viewspace);
}





void drag_drop_texture( std::string label, const std::string &payloadname, std::string &texture )
{
    label = label + " " + std::string(ICON_FA_DOWNLOAD);

    if (texture != "")
    {
        label = texture;
    }

    ImGui::ButtonEx(label.c_str(), ImVec2(250, 50), ImGuiButtonFlags_None);

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_DRAG_DROP"))
        {
            std::string filepath(reinterpret_cast<char *>(payload->Data));
            texture = filepath;
        }
        ImGui::EndDragDropTarget();
    }
}




template <>
void
EditorUI_MD::drawComponent<idk::StaticHeightmapCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<idk::StaticHeightmapCmp>(obj_id);

    ImGui::Text("Model ID: %d", cmp.model);
    ImGui::Separator();

    // Model drag-drop
    // -----------------------------------------------------------------------------------------
    if (cmp.textures.size() == 0)
    {
        cmp.textures.push_back("");
    }

    if (cmp.textures.back() != "")
    {
        cmp.textures.push_back("");
    }

    for (int i=0; i<cmp.textures.size(); i++)
    {
        drag_drop_texture("Texture", "ASSET_BROWSER_DRAG_DROP", cmp.textures[i]);
    }
    // -----------------------------------------------------------------------------------------


}



template <>
void
EditorUI_MD::drawComponent<idk::IconCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &data = ecs.getComponent<idk::IconCmp>(obj_id);
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


// template <>
// void
// EditorUI_MD::drawComponent<idk::ScriptCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
// {
//     auto &engine = api.getEngine();
    
//     auto &cmp    = ecs.getComponent<idk::ScriptCmp>(obj_id);
//     float width  = ImGui::GetWindowWidth() / 2.0f;

//     ImGui::Checkbox("Enable", &cmp.enabled);

//     // Script drag-drop
//     // -----------------------------------------------------------------------------------------
//     std::string label = "Script " ICON_FA_DOWNLOAD;
//     std::string &filepath = cmp.filepath;

//     if (filepath != "")
//     {
//         std::string filename = fs::path(filepath).stem();
//         label = filename;
//     }

//     ImGui::Button(label.c_str(), ImVec2(-FLT_MIN, 25));

//     if (ImGui::BeginDragDropTarget())
//     {
//         if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCRIPT_DRAG_DROP"))
//         {
//             cmp.filepath = std::string((char *)(payload->Data));
//             cmp.nparams = idk::ScriptCmp_getNumParams(cmp);
//         }
//         ImGui::EndDragDropTarget();
//     }
//     // -----------------------------------------------------------------------------------------

//     if (cmp.nparams == 1)
//     {
//         width *= 2.0f;
//     }

//     if (cmp.nparams >= 1)
//     {
//         // Subject drag-drop
//         // -----------------------------------------------------------------------------------------
//         label = "Subject " ICON_FA_DOWNLOAD;

//         if (cmp.subject_id != -1)
//         {
//             if (ecs.gameObjectExists(cmp.subject_id))
//             {
//                 label = ecs.getGameObjectName(cmp.subject_id);
//             }

//             else
//             {
//                 cmp.subject_id = -1;
//             }
//         }

//         ImGui::ButtonEx(label.c_str(), ImVec2(width, 50), ImGuiButtonFlags_None);

//         if (ImGui::BeginDragDropTarget())
//         {
//             if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
//             {
//                 IM_ASSERT(payload->DataSize == sizeof(int));
//                 cmp.subject_id = *reinterpret_cast<int *>(payload->Data);
//             }
//             ImGui::EndDragDropTarget();
//         }
//         // -----------------------------------------------------------------------------------------
//     }


//     if (cmp.nparams == 2)
//     {
//         ImGui::SameLine();

//         // Target drag-drop
//         // -----------------------------------------------------------------------------------------
//         label = "Target " ICON_FA_DOWNLOAD;
//         if (cmp.target_id != -1)
//         {
//             label = ecs.getGameObjectName(cmp.target_id);
//         }

//         ImGui::ButtonEx(label.c_str(), ImVec2(width, 50), ImGuiButtonFlags_None);

//         if (ImGui::BeginDragDropTarget())
//         {
//             if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
//             {
//                 IM_ASSERT(payload->DataSize == sizeof(int));
//                 cmp.target_id = *reinterpret_cast<int *>(payload->Data);
//             }
//             ImGui::EndDragDropTarget();
//         }
//         // -----------------------------------------------------------------------------------------
//     }


//     // Target drag-drop
//     // -----------------------------------------------------------------------------------------
//     label = "Depends on " ICON_FA_DOWNLOAD;
//     if (cmp.dependency != -1)
//     {
//         label = std::string("Depends on ") + ecs.getGameObjectName(cmp.target_id);
//     }

//     ImGui::ButtonEx(label.c_str(), ImVec2(width, 50), ImGuiButtonFlags_None);

//     if (ImGui::BeginDragDropTarget())
//     {
//         if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
//         {
//             IM_ASSERT(payload->DataSize == sizeof(int));
//             cmp.dependency = *reinterpret_cast<int *>(payload->Data);
//         }
//         ImGui::EndDragDropTarget();
//     }
//     // -----------------------------------------------------------------------------------------


//     // ImGui::PopStyleColor();
// }




template <>
void
EditorUI_MD::drawComponent<idk::AudioEmitterCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<idk::AudioEmitterCmp>(obj_id);
    std::string label = cmp.filepath;

    auto &em = api.getAudio().getEmitter(cmp.emitter_id);
    ImGui::InputFloat("Linear",    &em.att[1]);
    ImGui::InputFloat("Quadratic", &em.att[2]);


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
EditorUI_MD::drawComponent<idk::AudioListenerCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{

}



// template <>
// void
// EditorUI_MD::drawComponent<idk::ProgressionEventCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
// {
//     auto &engine = api.getEngine();
    
//     auto &cmp    = ecs.getComponent<idk::ProgressionEventCmp>(obj_id);
//     float width  = ImGui::GetWindowWidth() / 2.0f;

//     int value = int(cmp.prereq_progress);
//     ImGui::InputInt("Prerequisite progress", &value);
//     cmp.prereq_progress = uint32_t(value);


//     // Script drag-drop
//     // -----------------------------------------------------------------------------------------
//     std::string label = "Script " ICON_FA_DOWNLOAD;

//     if (cmp.script_id != -1)
//     {
//         label = ecs.getComponent<idk::ScriptCmp>(cmp.script_id).filepath;
//     }

//     ImGui::Button(label.c_str(), ImVec2(-FLT_MIN, 25));

//     if (ImGui::BeginDragDropTarget())
//     {
//         if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
//         {
//             IM_ASSERT(payload->DataSize == sizeof(int));
//             cmp.script_id = *reinterpret_cast<int *>(payload->Data);
//         }
//         ImGui::EndDragDropTarget();
//     }
//     // -----------------------------------------------------------------------------------------


//     // State drag-drop
//     // -----------------------------------------------------------------------------------------
//     label = "State " ICON_FA_DOWNLOAD;
//     if (cmp.state_id != -1)
//     {
//         label = ecs.getGameObjectName(cmp.state_id);
//     }

//     ImGui::ButtonEx(label.c_str(), ImVec2(width, 50), ImGuiButtonFlags_None);

//     if (ImGui::BeginDragDropTarget())
//     {
//         if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
//         {
//             IM_ASSERT(payload->DataSize == sizeof(int));
//             cmp.state_id = *reinterpret_cast<int *>(payload->Data);
//         }
//         ImGui::EndDragDropTarget();
//     }
//     // -----------------------------------------------------------------------------------------

// }


// template <>
// void
// EditorUI_MD::drawComponent<idk::ProgressionStateCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
// {
//     auto &cmp = ecs.getComponent<idk::ProgressionStateCmp>(obj_id);
//     int value = int(cmp.progress);
//     ImGui::InputInt("Progression State", &value);
//     cmp.progress = uint32_t(value);

//     for (int i=0; i<16; i++)
//     {
//         value = int(cmp.states[i]);
//         std::string label = "State " + std::to_string(i);
//         ImGui::InputInt(label.c_str(), &value);
//         cmp.states[i] = uint32_t(value);
//     }

// }




template <>
void
EditorUI_MD::drawComponent<idk::ScriptCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &ssys = ecs.getSystem<idk::ScriptSys>();
    auto &cmp  = ecs.getComponent<idk::ScriptCmp>(obj_id);

    ImGui::BeginTable("Scripts", 4, ImGuiTableFlags_SizingStretchSame);
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::TableNextColumn();

        if (ImGui::Button("Reload All"))
        {
            for (auto &name: cmp.scripts)
            {
                ssys.reloadScript(name);
            }
        }

        for (int i=0; i<cmp.scripts.size(); i++)
        {
            ImGui::PushID(i);
            ImGui::TableNextRow();
        
            ImGui::TableNextColumn();
            ImGui::Text(cmp.scripts[i].c_str());
        
            ImGui::TableNextColumn();

            std::string label = "Reload";

            {
                auto *script = ssys.getScript(cmp.scripts[i]);

                if (script->is_ready() == false)
                {
                    label = "Reloading... " ICON_FA_ROTATE;
                }
            }

            if (ImGui::Button(label.c_str()))
            {
                ssys.reloadScript(cmp.scripts[i]);
            }

            ImGui::TableNextColumn();
            if (ImGui::Button("Remove"))
            {
                std::swap(cmp.scripts[i], cmp.scripts.back());
                std::swap(cmp.data[i],    cmp.data.back());

                cmp.scripts.pop_back();
                cmp.data.pop_back();

                ImGui::PopID();
                break;
            }

            ImGui::PopID();
        }

        ImGui::EndTable();
    }

    std::string label = "Drag/Drop " ICON_FA_DOWNLOAD;
    ImGui::Button(label.c_str(), ImVec2(-FLT_MIN, 25));

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_DRAG_DROP"))
        {
            std::string filepath(reinterpret_cast<char *>(payload->Data));
            fs::path path(filepath);
            ssys.attachScript(obj_id, path.filename(), nullptr);
        }
        ImGui::EndDragDropTarget();
    }
    // ImGui::ButtonEx(ICON_FA_DOWNLOAD, ImVec2(250, 50), ImGuiButtonFlags_None);

    // if (ImGui::BeginDragDropTarget())
    // {
    //     if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_DRAG_DROP"))
    //     {
    //         std::string filepath(static_cast<char *>(payload->Data));
        
    //         if (fs::path(filepath).extension().string() == ".cpp")
    //         {
    //             cmp.scripts.push_back(idk::RuntimeScript(filepath));
    //             cmp.data.push_back(nullptr);
    //             cmp.timers.push_back(0);
    //         }
    //     }
    //     ImGui::EndDragDropTarget();
    // }
}





template <>
void
EditorUI_MD::drawComponent<idk::PhysicsCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<idk::PhysicsCmp>(obj_id);

    if (ImGui::Button("Bake mesh"))
    {
        idk::PhysicsSys::bakeMeshCollider(obj_id);
    }

    // ImGui::InputFloat3("Linear",  &cmp.linear[0], "%0.2f");
    // ImGui::InputFloat3("Angular", &cmp.angular[0], "%0.2f");
}


template <>
void
EditorUI_MD::drawComponent<idk::KinematicRectCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<idk::KinematicRectCmp>(obj_id);
    ImGui::Checkbox("Visualise", &cmp.visualise);

}


template <>
void
EditorUI_MD::drawComponent<idk::KinematicCapsuleCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<idk::KinematicCapsuleCmp>(obj_id);

    ImGui::Checkbox("Enable",    &cmp.enabled);
    ImGui::Checkbox("Visualise", &cmp.visualise);

    ImGui::DragFloat("radius", &cmp.radius, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("bottom", &cmp.bottom, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("top",    &cmp.top,    0.01f, 0.0f, 1.0f);

}



template <>
void
EditorUI_MD::drawComponent<idk::StaticRectCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<idk::StaticRectCmp>(obj_id);

    ImGui::Checkbox("Visualise", &cmp.visualise);
}



template <>
void
EditorUI_MD::drawComponent<idk::RenderSettingCmp>( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &engine  = api.getEngine();
    auto &ren     = api.getRenderer();
    auto &cmp     = ecs.getComponent<idk::RenderSettingCmp>(obj_id);
    auto settings = ren.getRenderSettings();

    std::string filepath = cmp.filepath;

    // auto &config = ren.getRenderConfig();

    bool B = false;

    if (ImGui::BeginTabBar("Render Settings", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("Volumetrics"))
        {
            auto &config = settings.volumetrics;
            B |= ImGui::Checkbox("Enable", &config.enabled);

            B |= ImGui::InputInt   ("View samples", &config.samples);
            B |= ImGui::InputInt   ("Sun samples",  &config.samples_sun);
            B |= ImGui::SliderInt  ("Res divisor",  &config.res_divisor, 1, 8);
            B |= ImGui::SliderInt  ("Blend mode",   &config.blend_mode, 0, 2);
            B |= ImGui::SliderFloat("Intensity",    &config.intensity, 0.0f, 2.0f);

            B |= ImGui::DragFloat("Height offset",  &config.height_offset,  0.01f,  0.01f);
            B |= ImGui::DragFloat("Height falloff", &config.height_falloff, 0.001f, 0.0f);
            B |= ImGui::DragFloat("Scatter coeff",  &config.scatter_coeff, 0.01f, 0.0f, 10.0f);
            B |= ImGui::DragFloat("Absorb coeff",   &config.absorb_coeff, 0.01f, 0.0f, 10.0f);

            B |= ImGui::DragFloat("Worley amp", &config.worley_amp, 0.01f, 0.01f, +1.0f);
            B |= ImGui::DragFloat("Worley Wav", &config.worley_wav, 0.01f, 0.01f, +1.0f);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("SSAO"))
        {
            auto &config = settings.ssao;
            B |= ImGui::Checkbox("Enable", &config.enabled);
            B |= ImGui::InputInt("Samples",     &config.samples);
            B |= ImGui::InputFloat("Factor", &config.factor);
            B |= ImGui::InputFloat("Intensity", &config.intensity);
            B |= ImGui::InputFloat("Radius",    &config.radius);
            B |= ImGui::InputFloat("Bias",      &config.bias);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("SSR"))
        {
            auto &config = settings.ssr;
            B |= ImGui::Checkbox("Enable",      &config.enabled);
            B |= ImGui::InputInt("Blend mode",  &config.blend_mode);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("SSGI"))
        {
            auto &config = settings.ssgi;
            B |= ImGui::Checkbox("Enable", &config.enabled);
            B |= ImGui::InputInt("Factor", &config.factor);
            B |= ImGui::InputFloat("Intensity", &config.intensity);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("TAA"))
        {
            auto &config = settings.taa;
            B |= ImGui::Checkbox("Enable", &config.enabled);
            B |= ImGui::InputInt("Factor",  &config.factor);
            B |= ImGui::InputFloat("Scale", &config.scale);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Skybox"))
        {
            if (ImGui::InputText("Skybox", &filepath, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (filepath != cmp.filepath)
                {
                    cmp.filepath = filepath;
                    // idk::RenderSettingSys::loadSkybox(filepath);
                }
            }
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    // if (B)
    // {
        ren.applyRenderSettings(settings);
    // }

}





// #define ECS_COMPONENT_CALLBACK(component_type) \
// { \
//     ecs.getComponentArray<component_type>().userCallback = \
//         EditorUI_MD::drawComponent<component_type> \
//     ; \
// }


#define ECS_COMPONENT_CALLBACK(component_type) \
{ \
    idk::ECS::setUserCallback<component_type>( \
        EditorUI_MD::drawComponent<component_type> \
    ); \
} \



void
EditorUI_MD::registerDrawComponents()
{
    ECS_COMPONENT_CALLBACK(idk::IconCmp);
    ECS_COMPONENT_CALLBACK(idk::TransformCmp);
    ECS_COMPONENT_CALLBACK(idk::IKCmp);
    ECS_COMPONENT_CALLBACK(idk::LookTowardCmp);
    ECS_COMPONENT_CALLBACK(idk::SmoothFollowCmp);
    ECS_COMPONENT_CALLBACK(idk::AnchorCmp);
    ECS_COMPONENT_CALLBACK(idk::RotateCmp);

    ECS_COMPONENT_CALLBACK(idk::ModelCmp);
    ECS_COMPONENT_CALLBACK(idk::TerrainCmp);
    // ECS_COMPONENT_CALLBACK(idk::StaticHeightmapCmp);
    ECS_COMPONENT_CALLBACK(idk::CameraCmp);

    // ECS_COMPONENT_CALLBACK(idk::PhysicsCmp);
    // ECS_COMPONENT_CALLBACK(idk::StaticRectCmp);
    // ECS_COMPONENT_CALLBACK(idk::KinematicRectCmp);
    // ECS_COMPONENT_CALLBACK(idk::KinematicCapsuleCmp);

    ECS_COMPONENT_CALLBACK(idk::DirlightCmp);
    ECS_COMPONENT_CALLBACK(idk::PointlightCmp);
    ECS_COMPONENT_CALLBACK(idk::SpotlightCmp);

    ECS_COMPONENT_CALLBACK(idk::AudioEmitterCmp);

    ECS_COMPONENT_CALLBACK(idk::RenderSettingCmp);
    ECS_COMPONENT_CALLBACK(idk::ScriptCmp);
    ECS_COMPONENT_CALLBACK(idk::ParticleCmp);

}


void
EditorUI_MD::clearDrawComponents()
{
    using namespace idk;

    ECS::clearUserCallback<idk::IconCmp>();
    ECS::clearUserCallback<idk::TransformCmp>();
    ECS::clearUserCallback<idk::IKCmp>();
    ECS::clearUserCallback<idk::LookTowardCmp>();
    ECS::clearUserCallback<idk::SmoothFollowCmp>();
    ECS::clearUserCallback<idk::AnchorCmp>();
    ECS::clearUserCallback<idk::RotateCmp>();
    ECS::clearUserCallback<idk::ModelCmp>();
    ECS::clearUserCallback<idk::TerrainCmp>();
    ECS::clearUserCallback<idk::CameraCmp>();
    ECS::clearUserCallback<idk::DirlightCmp>();
    ECS::clearUserCallback<idk::PointlightCmp>();
    ECS::clearUserCallback<idk::SpotlightCmp>();
    ECS::clearUserCallback<idk::AudioEmitterCmp>();
    ECS::clearUserCallback<idk::RenderSettingCmp>();
    ECS::clearUserCallback<idk::ScriptCmp>();
    ECS::clearUserCallback<idk::ParticleCmp>();

}


