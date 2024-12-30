#include "../EditorUI.hpp"
#include <IDKGraphics/terrain/terrain.hpp>
#include <IDKGraphics/noise/noise.hpp>


static int   w     = 512.0f;
static float ratio = 1.0f;


static void
display_texture( const std::string &name, uint32_t texture, float r=ratio )
{
    int width  = int(ImGui::GetContentRegionAvail().x);
    int height = float(width) / ratio;

    ImGui::Text(name.c_str());

    uint64_t img0 = texture;
    void *img1 = (void *)(&img0);

    ImGui::Image(
        *((ImTextureID *)img1),
        ImVec2(width, height),
        ImVec2(0.0f, 1.0f),
        ImVec2(1.0f, 0.0f)
    );
}


static void
display_textures( const std::vector<GLenum> &textures, int row_w=1, float r=ratio )
{
    int width  = int(ImGui::GetContentRegionAvail().x) / row_w;
    int height = float(width) / r;

    for (int i=0; i<textures.size(); i++)
    {
        uint64_t img0 = textures[i];
        void *img1 = (void *)(&img0);

        ImGui::Image(
            *((ImTextureID *)img1),
            ImVec2(width, height),
            ImVec2(0.0f, 1.0f),
            ImVec2(1.0f, 0.0f)
        );

        ImGui::SameLine();

        if ((i+1) % row_w == 0 || (row_w==1 && i%1 == 0))
        {
            ImGui::Dummy(ImVec2(0, 0));
        }
    }

    ImGui::Dummy(ImVec2(0, 0));
}



void
EditorUI_MD::_tab_buffers( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    ratio = float(ren.width()) / ren.height();
    w = int(ImGui::GetContentRegionAvail().x);

    ImGui::Begin("Buffers");

    if (ImGui::BeginTabBar("Render Buffers", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("G Buffer"))
        {
            display_textures(ren.m_gbuffers[0]->attachments);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("History Buffer"))
        {
            display_textures(ren.m_lightbuffers[0]->attachments);
            display_textures(ren.m_lightbuffers[1]->attachments);
            display_textures(ren.m_lightbuffers[2]->attachments);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Shadow map"))
        {
            display_textures(ren.m_dirshadow2_buffer.attachments, 2, 1.0f);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("SSR"))
        {
            display_textures(ren.m_mipbuffer.attachments, 3);
            display_textures(ren.m_SSGI_buffers[0]->attachments);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("SSS"))
        {
            display_textures(ren.m_shadow_buffer.attachments, 1);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("SSAO"))
        {
            display_textures(ren.m_SSAO_buffers[0]->attachments);
            ImGui::EndTabItem();
        }
    
        if (ImGui::BeginTabItem("Volumetrics"))
        {
            display_textures(ren.m_volumetrics_buffer.attachments);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Terrain"))
        {
            std::vector<uint32_t> textures = {
                idk::TerrainRenderer::getHeightMap(),
                idk::TerrainRenderer::getNormalMap()
            };

            display_textures(textures, 2, 1.0f);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("UI"))
        {
            display_textures(ren.getUIFrameBuffer().attachments);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();

}
