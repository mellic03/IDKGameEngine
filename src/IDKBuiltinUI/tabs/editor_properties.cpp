#include "../EditorUI.hpp"

#include <IDKGraphics/idk_render_settings.hpp>
#include <IDKGraphics/terrain/terrain.hpp>



void
EditorUI_MD::_tab_editor_properties( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();


    ImGui::Begin("Editor Properties");

    if (ImGui::Button("Recompile shaders"))
    {
        ren.recompileShaders();
    }


    ImGui::Text("Translation snapping");
    ImGui::InputFloat("##A", &m_tsnap, 0.25f, 0.25f, "%.2f", ImGuiInputTextFlags_None);

    ImGui::Text("Rotation snapping");
    ImGui::InputFloat("##B", &m_rsnap, 10.0f, 5.0f, "%.2f", ImGuiInputTextFlags_None);

    ImGui::Text("Scale snapping");
    ImGui::InputFloat("##C", &m_ssnap, 0.0f, 2.0f, "%.2f", ImGuiInputTextFlags_None);


    // auto settings = ren.getRenderSettings();


    // {
    //     ImGui::Dummy({0, 10});
    //     ImGui::SeparatorText("Water");

    //     auto &temp = idk::TerrainRenderer::getWaterTemp();

    //     ImGui::DragFloat("X scale", &temp.xscale, 0.1f, 0.0f, 5.0f);
    //     ImGui::DragFloat("Y scale", &temp.yscale, 0.1f, 0.0f, 5.0f);
    //     ImGui::DragFloat("t scale", &temp.tscale, 0.1f, 0.0f, 5.0f);
    //     ImGui::DragFloat("Amp factor", &temp.amp_factor, 0.01f, 0.0f, 5.0f);
    //     ImGui::DragFloat("Wav factor", &temp.wav_factor, 0.01f, 0.0f, 5.0f);
    //     ImGui::DragFloat("Hoz scale",  &temp.hoz_scale,  0.01f, 0.0f, 5.0f);
    //     ImGui::DragFloat4("Mul factors", &temp.mul_factors[0], 0.1f, -5.0f, 5.0f);
    //     ImGui::InputInt("Waves", &temp.waves);
    // }

    // {
    //     ImGui::Dummy({0, 10});
    //     ImGui::SeparatorText("SSAO");

    //     auto &config = settings.ssao;

    //     ImGui::Checkbox("Enable ##SSAO", &config.enabled);
    
    //     ImGui::InputFloat("Factor",    &config.factor);
    //     ImGui::InputFloat("Intensity", &config.intensity);
    //     ImGui::InputFloat("Radius",    &config.radius);
    //     ImGui::InputFloat("Bias",      &config.bias);

    // }

    // {
    //     ImGui::Dummy({0, 10});
    //     ImGui::SeparatorText("Volumetrics");
    
    //     auto &config = settings.volumetrics;

    //     ImGui::Checkbox("Enable ##VOL", &config.enabled);
    //     ImGui::InputFloat("Factor ##VOL", &config.factor);
    //     ImGui::InputFloat("Attenuation ##VOL", &config.attenuation);
    //     ImGui::InputFloat("Intensity ##VOL", &config.intensity);
    //     ImGui::InputFloat("Samples ##VOL", &config.samples);
    // }

    // {
    //     ImGui::Dummy({0, 10});
    //     ImGui::SeparatorText("Environment Mapping");
    
    //     auto &config = settings.envprobe;

    //     ImGui::Checkbox("Enable ##ENV",    &config.enabled);
    //     ImGui::Checkbox("Visualize", &config.visualize);

    //     ImGui::InputInt3   ("Grid size",  &(config.grid_size[0]));
    //     ImGui::InputFloat3 ("Probe size", &(config.cell_size[0]));
    // }

    // ren.applyRenderSettings(settings);


    ImGui::End();
}
