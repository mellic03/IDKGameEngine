#include "EditorUI-tabs.hpp"



static void callback( IDK_Camera &cam )
{
    static glm::vec3 offset(0.5f, 1.0f, 1.0f);
    // idkgui::dragVec3("Offset", &offset[0], -2.0f, 2.0f, 0.1f, "%.1f", 0.0f);
    // cam.setOffset(offset);


}



void
EditorTab::cameras( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    static int selected = -1;


    ImGui::Begin("Cameras");

    if (idkImGui::splitWindow_begin("Cameras"))
    {
        int n = 0;

        // for (IDK_Camera &cam: ren.getCameras())
        // {
        //     std::string label = std::to_string(n);


        //     if (ImGui::TreeNodeEx(label.c_str(), ImGuiTreeNodeFlags_Leaf))
        //     {
        //         if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        //         {
        //             ImGui::SetDragDropPayload("CAMERA_DRAG_DROP",
        //                 (const void *)(&n),
        //                 sizeof(int)
        //             );

        //             if (ImGui::IsDragDropActive())
        //             {
        //                 ImGui::Text(label.c_str());
        //             }
        //             ImGui::EndDragDropSource();
        //         }
        //         ImGui::TreePop();
        //     }

        //     if (ImGui::Selectable(label.c_str(), selected == n))
        //     {
        //         selected = n;
        //     }
     
        //     n += 1;
        // }

        idkImGui::splitWindow_split();

        // if (selected != -1)
        // {
        //     IDK_Camera &cam = ren.getCamera(selected);

        //     if (ImGui::Button("Switch to"))
        //     {
        //         ren.useCamera(selected);
        //     }

        //     ImGui::Spacing();

        //     // ImGui::SliderFloat("Strength", &cam.m_abr_str, 0.0f, 0.1f, "%.4f");
        //     // ImGui::SliderFloat2("R",  &cam.m_r_abr.x,  -1.0f, 1.0f, "%.0f");
        //     // ImGui::SameLine();
        //     // if (ImGui::Button("RReset"))
        //     //     cam.m_r_abr = glm::vec2(0.0f);

        //     // ImGui::SliderFloat2("G",  &cam.m_g_abr.x,  -1.0f, 1.0f, "%.1f");
        //     // ImGui::SameLine();
        //     // if (ImGui::Button("GReset"))
        //     //     cam.m_g_abr = glm::vec2(0.0f);

        //     // ImGui::SliderFloat2("B",  &cam.m_b_abr.x,  -1.0f, 1.0f, "%.1f");
        //     // ImGui::SameLine();
        //     // if (ImGui::Button("BReset"))
        //     //     cam.m_b_abr = glm::vec2(0.0f);

        //     // ImGui::Spacing();
        // }

        idkImGui::splitWindow_end();
    }

    ImGui::End();

}

