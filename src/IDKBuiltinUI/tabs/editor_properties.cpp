#include "../EditorUI.hpp"


void
EditorUI_Module::_tab_editor_properties( idk::EngineAPI &api )
{
    ImGui::Begin("Editor Properties");

    ImGui::Text("Translation snapping");
    ImGui::InputFloat("##A", &m_tsnap, 0.25f, 0.25f, "%.2f", ImGuiInputTextFlags_None);

    ImGui::Text("Rotation snapping");
    ImGui::InputFloat("##B", &m_rsnap, 10.0f, 5.0f, "%.2f", ImGuiInputTextFlags_None);
    
    ImGui::End();
}
