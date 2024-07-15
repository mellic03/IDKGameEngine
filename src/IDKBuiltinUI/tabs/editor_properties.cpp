#include "../EditorUI.hpp"

#include <IDKGraphics/idk_render_settings.hpp>



void
EditorUI_MD::_tab_editor_properties( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();


    ImGui::Begin("Editor Properties");

    ImGui::Text("Translation snapping");
    ImGui::InputFloat("##A", &m_tsnap, 0.25f, 0.25f, "%.2f", ImGuiInputTextFlags_None);

    ImGui::Text("Rotation snapping");
    ImGui::InputFloat("##B", &m_rsnap, 10.0f, 5.0f, "%.2f", ImGuiInputTextFlags_None);

    ImGui::Text("Scale snapping");
    ImGui::InputFloat("##C", &m_ssnap, 0.0f, 2.0f, "%.2f", ImGuiInputTextFlags_None);


    {
        ImGui::Text("Environment Mapping");

        auto settings = ren.getRenderSettings();
        auto &config = settings.envprobe;

        ImGui::Checkbox("Enable",    &config.enabled);
        ImGui::Checkbox("Visualize", &config.visualize);

        ImGui::InputInt3   ("Grid size",  &(config.grid_size[0]));
        ImGui::InputFloat3 ("Probe size", &(config.cell_size[0]));

        ren.applyRenderSettings(settings);

        // static bool probe_vis = false;
        // ImGui::Checkbox("Visualize light probes", &probe_vis);
        // ren.setRenderSetting(idk::RenderSetting::LIGHTPROBE_VIS, probe_vis);
    }


    {
        // auto &settings = ren.getRenderSettings();
        // auto &config = settings.envprobe;

        // ImGui::Text("SSAO");

        // ImGui::Separator();
        // ImGui::Checkbox("Enable SSAO", &settings.enabled);
        // ImGui::InputFloat("Intensity", &settings.intensity);
        // ImGui::InputFloat("Factor",    &settings.factor);


        // ImGui::InputInt("Samples",   &settings.samples);
        // ImGui::InputFloat("Radius",  &settings.radius);
        // ImGui::InputFloat("Bias",    &settings.bias);
    }

    // ImGui::Spacing();
    // ImGui::Checkbox("VXGI Debug", &api.getRenderer().m_vxgi_debug);

    ImGui::End();
}
