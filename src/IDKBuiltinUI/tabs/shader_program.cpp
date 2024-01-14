#include "EditorUI-tabs.hpp"

#include <idk_imgui/imgui.hpp>




void
tab_shader_program_edit( const std::string &name, idk::glShaderProgram &program )
{
    if (name == "")
    {
        return;
    }

    for (auto &[name, def]: program.getDefinitions())
    {
        ImGui::InputText(name.c_str(), &def.value);
    }

    if (ImGui::Button("Recompile"))
    {
        program.compile();
    }

}



void
EditorTab::shader_programs( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    auto &programs = ren.getPrograms();
    static std::string selected = "";

    ImGui::Begin("Shader Programs");

    if (ImGui::BeginTable("table1", 2, EditorTab::resizeable_table_flags))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        for (auto &[name, program]: programs)
        {
            if (ImGui::Selectable(name.c_str(), selected == name))
            {
                selected = name;
            }
        }

        ImGui::TableNextColumn();
        tab_shader_program_edit(selected, programs[selected]);
        
        ImGui::EndTable();
    }

    ImGui::End();
}
