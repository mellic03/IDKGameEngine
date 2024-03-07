#include "../EditorUI.hpp"


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


    static bool flags[32];
    static bool first = true;

    if (first)
    {
        for (int i=0; i<32; i++)
        {
            flags[i] = ren.getRenderSetting(static_cast<idk::RenderSetting>(1 << i));
        }

        first = false;
    }

    for (int i=0; i<32; i++)
    {
        std::string label = std::to_string(i);
        ImGui::Checkbox(label.c_str(), &flags[i]);
        ren.setRenderSetting(static_cast<idk::RenderSetting>(1 << i), flags[i]);

        if (i % 5 != 0)
        {
            ImGui::SameLine();
        }
    }

    ImGui::Spacing();
    ImGui::Checkbox("VXGI Debug", &api.getRenderer().m_vxgi_debug);

    ImGui::End();
}
