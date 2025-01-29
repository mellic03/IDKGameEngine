#include "../EditorUI.hpp"


void
idk::EditorUI::_menubar_settings( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    if (ImGui::BeginMenu("Settings"))
    {
        if (ImGui::BeginMenu("Font Size"))
        {
            for (int i=0; i<NUM_FONTS; i++)
            {
                int font_size = FONT_SIZES[i];
                std::string label = std::to_string(font_size) + "px";

                if (ImGui::MenuItem(label.c_str()))
                {
                    ImGui::GetIO().FontDefault = m_fonts[i];
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::MenuItem("ImGui Demo Window"))
        {
            m_demowindow = true;
        }

        ImGui::EndMenu();
    }
}

