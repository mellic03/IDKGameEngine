#include "idk_imgui_extra.hpp"
#include <idk_icons/idk_Icons.hpp>

#include <filesystem>

void
EditorUI::dragVec3( std::string name, float *data, float min, float max,
                    float speed, const char *format, float default_value )
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});

    ImGui::BeginTable(name.c_str(), 4, ImGuiTableFlags_SizingStretchSame);
    {
        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        ImGui::Text(name.c_str());

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});

            float line_height = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
            ImVec2 button_size = { line_height + 3.0f, line_height };

            ImGui::TableNextColumn();
            if (ImGui::Button("X", button_size))
                data[0] = default_value;
            ImGui::SameLine();
            ImGui::DragFloat("##X", &data[0], speed, min, max, format);
            
            ImGui::TableNextColumn();
            if (ImGui::Button("Y", button_size))
                data[1] = default_value;
            ImGui::SameLine();
            ImGui::DragFloat("##Y", &data[1], speed, min, max, format);

            ImGui::TableNextColumn();
            if (ImGui::Button("Z", button_size))
                data[2] = default_value;
            ImGui::SameLine();
            ImGui::DragFloat("##Z", &data[2], speed, min, max, format);

        ImGui::PopStyleVar();

        ImGui::EndTable();
    }

    ImGui::PopStyleVar();
}



static void
file_select( const std::string &path, std::string &selection )
{
    using namespace std::filesystem;

    static const auto dir_flags  = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnArrow;
    static const auto file_flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf;

    for (auto &dir_entry: directory_iterator(path))
    {
        if (dir_entry.is_directory())
        {
            std::string entry_path = dir_entry.path();
            std::string entry_name = dir_entry.path().filename();
            std::string label = ICON_FA_FOLDER " " + entry_name;

            int flags = dir_flags;

            if (entry_path == selection)
            {
                flags |= ImGuiTreeNodeFlags_Selected;
            }

            bool node_open = ImGui::TreeNodeEx(label.c_str(), flags);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            {
                selection = entry_path;
            }

            if (node_open)
            {
                file_select(entry_path, selection);
                ImGui::TreePop();
            }

        }
    }


    for (auto &dir_entry: directory_iterator(path))
    {
        std::string entry_path = dir_entry.path();
        std::string entry_name = dir_entry.path().filename();
        std::string label = ICON_FA_FILE " " + entry_name;

        if (dir_entry.is_directory() == false)
        {
            int flags = file_flags;

            if (entry_path == selection)
            {
                flags |= ImGuiTreeNodeFlags_Selected;
            }

            if (ImGui::TreeNodeEx(label.c_str(), flags))
            {
                if (ImGui::IsItemClicked())
                    selection = entry_path;

                ImGui::TreePop();
            }

        }
    }
}


bool
idkImGui::fileSelectPopup( const std::string &name, bool &open,
                           const std::string &entry_path, std::string &selection )
{
    bool result = false;

    if (ImGui::BeginPopupModal(name.c_str(), &open, 0))
    {

        if (idkImGui::splitWindowH_begin("fileSelectPopup_A"))
        {
            file_select( entry_path, selection );
    
            idkImGui::splitWindowH_split();

            if (ImGui::Button("Button A"))
            {

            }

            if (ImGui::Button("Close"))
            {
                open = false;
            }

            ImGui::SameLine();

            if (ImGui::Button("Open"))
            {
                open = false;
                result = true;
            }

            idkImGui::splitWindowH_end();
        }

        ImGui::EndPopup();
    }

    return result;

}








bool
idkImGui::splitWindow_begin( const std::string &label )
{
    constexpr auto flags = ImGuiTableFlags_SizingStretchSame
                         | ImGuiTableFlags_Resizable
                         | ImGuiTableFlags_BordersOuter
                         | ImGuiTableFlags_BordersV;

    if (ImGui::BeginTable(label.c_str(), 2, flags))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::BeginChild("splitWindow_begin");

        return true;
    }

    return false;
}


void
idkImGui::splitWindow_split()
{
    ImGui::EndChild();
    ImGui::SameLine();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
    ImGui::TableNextColumn();

    ImGui::BeginChild("splitWindow_split");
}


void
idkImGui::splitWindow_end()
{
    ImGui::EndChild();
    ImGui::EndTable();
    ImGui::PopStyleVar();
}






bool
idkImGui::splitWindowH_begin( const std::string &label )
{
    constexpr auto flags = ImGuiTableFlags_SizingStretchSame
                         | ImGuiTableFlags_Resizable
                         | ImGuiTableFlags_BordersOuter
                         | ImGuiTableFlags_BordersV;

    if (ImGui::BeginTable(label.c_str(), 1, flags))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::BeginChild("splitWindowH_begin");

        return true;
    }

    return false;
}


void
idkImGui::splitWindowH_split()
{
    ImGui::EndChild();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    ImGui::BeginChild("splitWindowH_split");
}


void
idkImGui::splitWindowH_end()
{
    ImGui::EndChild();
    ImGui::EndTable();
    ImGui::PopStyleVar();
}



