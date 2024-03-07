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

    const std::map<std::string, int> &programs = ren.getProgramIDs();
    static std::string selected = "";

    ImGui::Begin("Shader Programs");

    if (ImGui::BeginTable("table1", 2, EditorTab::resizeable_table_flags))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        for (auto &[name, id]: programs)
        {
            if (ImGui::Selectable(name.c_str(), selected == name))
            {
                selected = name;
            }
        }

        ImGui::TableNextColumn();

        if (selected != "")
        {
            tab_shader_program_edit(selected, ren.getProgram(selected));
        }
        
        ImGui::EndTable();
    }

    ImGui::End();
}
