#include "../EditorUI.hpp"



void
EditorUI_MD::_tab_buffers( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    // auto &buffers1 = ren.getScratchBuffers();
    auto &buffers2 = ren.getScratchBuffers2();
    float ratio = float(ren.width()) / ren.height();


    ImGui::Begin("Render Buffers");


    {
        int w = int(ImGui::GetContentRegionAvail().x);
    
        ImGui::Image(
            *(ImTextureID *)(void *)(&(ren.m_mip_scratchbuffer.attachments[0])),
            ImVec2(w, w/ratio),
            ImVec2(0.0f, 1.0f),
            ImVec2(1.0f, 0.0f)
        );

        ImGui::Image(
            *(ImTextureID *)(void *)(&(ren.m_geom_buffer.attachments[0])),
            ImVec2(w, w/ratio),
            ImVec2(0.0f, 1.0f),
            ImVec2(1.0f, 0.0f)
        );

        ImGui::Image(
            *(ImTextureID *)(void *)(&(ren.m_geom_buffer.attachments[1])),
            ImVec2(w, w/ratio),
            ImVec2(0.0f, 1.0f),
            ImVec2(1.0f, 0.0f)
        );

        ImGui::Image(
            *(ImTextureID *)(void *)(&(ren.m_geom_buffer.attachments[2])),
            ImVec2(w, w/ratio),
            ImVec2(0.0f, 1.0f),
            ImVec2(1.0f, 0.0f)
        );

        ImGui::Image(
            *(ImTextureID *)(void *)(&(ren.m_mainbuffer_0.attachments[0])),
            ImVec2(w, w/ratio),
            ImVec2(0.0f, 1.0f),
            ImVec2(1.0f, 0.0f)
        );
    
        ImGui::Image(
            *(ImTextureID *)(void *)(&(ren.m_dirshadow_buffer.depth_attachment)),
            ImVec2(w, w),
            ImVec2(0.0f, 1.0f),
            ImVec2(1.0f, 0.0f)
        );

    }

    for (int i=0; i<2; i++)
    {
        std::string label = "Buffer " + std::to_string(4+i);
        ImGui::Text(label.c_str());

        int w = int(ImGui::GetContentRegionAvail().x);

        ImGui::Image(
            *(ImTextureID *)(void *)(&(buffers2[i].attachments[0])),
            ImVec2(w, w/ratio),
            ImVec2(0.0f, 1.0f),
            ImVec2(1.0f, 0.0f)
        );
    }

    ImGui::End();
}
