#include "idk_imgui_assetbrowser.hpp"
#include <idk_icons/idk_Icons.hpp>
#include "idk_imgui_extra.hpp"

namespace fs = std::filesystem;


static bool
AssetBrowser_ShowDirectories( fs::directory_entry &current_dir,
                              std::set<fs::directory_entry> &folder_entries,
                              float size, int &count, int NUM_COLS )
{
    bool result = true;
    bool running = true;

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5, 0.5, 0.5, 1.0));

    for (auto dir_iter: folder_entries)
    {
        std::string label = dir_iter.path().filename();
        ImGui::TableNextColumn();

        ImGui::PushID(count);
        ImGui::BeginGroup();

        ImGui::SetWindowFontScale(2.0f);
        ImGui::Button(ICON_FA_FOLDER, ImVec2(size, size));
        ImGui::SetWindowFontScale(1.0f);
        
        if (ImGui::IsItemClicked(0) && ImGui::IsMouseDoubleClicked(0))
        {
            current_dir = dir_iter;
            result = false;
            running = false;
        }

        ImGui::TextWrapped(label.c_str());

        ImGui::EndGroup();
        ImGui::PopID();


        count += 1;
        if (count % NUM_COLS == 0)
        {
            ImGui::TableNextRow();
        }

        if (!running)
        {
            break;
        }
    }

    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();

    return result;
}


static void
AssetBrowser_ShowFiles( std::set<fs::directory_entry> &file_entries, float size, int &count, int NUM_COLS )
{
    for (auto dir_iter: file_entries)
    {
        std::string label = dir_iter.path().filename();
        ImGui::TableNextColumn();

        ImGui::PushID(count);

        ImGui::BeginGroup();
        ImGui::SetWindowFontScale(2.0f);
        ImGui::Button(ICON_FA_FILE, ImVec2(size, size));
        ImGui::SetWindowFontScale(1.0f);

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            static std::string filepath = "";
            filepath = dir_iter.path().string();

            ImGui::SetDragDropPayload(
                "ASSET_BROWSER_DRAG_DROP",
                reinterpret_cast<const void *>(filepath.c_str()),
                sizeof(char) * (filepath.length() + 1)
            );

            {
                ImGui::Button(ICON_FA_FILE, ImVec2(size, size));
                ImGui::TextWrapped(label.c_str());
            }
            ImGui::EndDragDropSource();
        }

        ImGui::TextWrapped(label.c_str());
        ImGui::EndGroup();

        ImGui::PopID();


        count += 1;
        if (count % NUM_COLS == 0)
        {
            ImGui::TableNextRow();
        }
    }
}


bool
idkImGui::AssetBrowser( const char *name, fs::directory_entry &current_dir )
{
    uint32_t table_flags = ImGuiTableFlags_Borders
                         | ImGuiTableFlags_RowBg
                         | ImGuiTableFlags_ScrollX
                         | ImGuiTableFlags_ScrollY;

    ImGui::Begin(name);

    float button_size = 75.0f;

    int NUM_COLS = int(ImGui::GetContentRegionAvail().x) / button_size;
        NUM_COLS = (NUM_COLS <= 0) ? 1 : NUM_COLS;

    if (ImGui::Button("Up"))
    {
        current_dir = fs::directory_entry(current_dir.path().parent_path());
    }


    static std::set<fs::directory_entry> folders;   folders.clear();
    static std::set<fs::directory_entry> files;     files.clear();

    for (auto dir_iter: fs::directory_iterator(current_dir))
    {
        if (dir_iter.is_directory())
            folders.emplace(dir_iter);

        else
            files.emplace(dir_iter);
    }


    if (ImGui::BeginTable("Icons", NUM_COLS, 0))
    {
        int count = 0;

        ImGui::TableNextRow();
        AssetBrowser_ShowDirectories(current_dir, folders, button_size, count, NUM_COLS);
        AssetBrowser_ShowFiles(files, button_size, count, NUM_COLS);
        ImGui::EndTable();
    }

    ImGui::End();

    return true;
}
