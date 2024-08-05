#include "../EditorUI.hpp"
#include <IDKGraphics/terrain/terrain.hpp>


static int   w     = 512.0f;
static float ratio = 1.0f;


static void
display_texture( const std::string &name, uint32_t texture, float r=ratio )
{
    ImGui::Text(name.c_str());

    ImGui::Image(
        *(ImTextureID *)(void *)(&(texture)),
        ImVec2(w, w/r),
        ImVec2(0.0f, 1.0f),
        ImVec2(1.0f, 0.0f)
    );
}



void
EditorUI_MD::_tab_buffers( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    // auto &buffers1 = ren.getScratchBuffers();
    auto &buffers2 = ren.getScratchBuffers2();

    ImGui::Begin("Render Buffers");

    {
        ratio = float(ren.width()) / ren.height();
        w = int(ImGui::GetContentRegionAvail().x);

        display_texture("Terrain Normal", idk::TerrainRenderer::getNormalMap(), 1.0f);
        display_texture("SSAO",     ren.m_SSAO_buffers[0].attachments[0]);
        display_texture("UI",       ren.getUIFrameBuffer().attachments[0]);

        display_texture("Albedo",   ren.m_gbuffers[0]->attachments[0]);
        display_texture("Normal",   ren.m_gbuffers[0]->attachments[1]);
        display_texture("AO_R_M",   ren.m_gbuffers[0]->attachments[2]);

        // ImGui::Image(
        //     *(ImTextureID *)(void *)(&(ren.m_mip_scratchbuffer.attachments[0])),
        //     ImVec2(w, w/ratio),
        //     ImVec2(0.0f, 1.0f),
        //     ImVec2(1.0f, 0.0f)
        // );

        // ImGui::Image(
        //     *(ImTextureID *)(void *)(&(ren.m_mainbuffer_0.attachments[0])),
        //     ImVec2(w, w/ratio),
        //     ImVec2(0.0f, 1.0f),
        //     ImVec2(1.0f, 0.0f)
        // );

    }

    // for (int i=0; i<2; i++)
    // {
    //     std::string label = "Buffer " + std::to_string(4+i);
    //     ImGui::Text(label.c_str());

    //     int w = int(ImGui::GetContentRegionAvail().x);

    //     ImGui::Image(
    //         *(ImTextureID *)(void *)(&(buffers2[i].attachments[0])),
    //         ImVec2(w, w/ratio),
    //         ImVec2(0.0f, 1.0f),
    //         ImVec2(1.0f, 0.0f)
    //     );
    // }

    ImGui::End();
}
