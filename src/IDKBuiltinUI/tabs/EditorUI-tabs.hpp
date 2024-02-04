#pragma once

#include <IDKGameEngine/IDKengine.hpp>
#include "../common/idk_imgui_extra.hpp"


namespace EditorUI
{
    template <typename It, typename lambda_t>
    void selection_splitWindow( const It &begin, const It &end, const lambda_t &callback );

    template <typename T, typename lambda_t>
    void selection_splitWindow2( idk::Allocator<T> &container, const lambda_t &callback );
};


namespace EditorTab
{
    void shader_programs( idk::EngineAPI & );
    void skybox    ( idk::EngineAPI & );
    void cameras   ( idk::EngineAPI & );
    void models    ( idk::EngineAPI & );
    void editor    ( idk::EngineAPI & );
    void dirlights ( idk::EngineAPI & );

    constexpr auto resizeable_table_flags =   ImGuiTableFlags_SizingStretchSame
                                            | ImGuiTableFlags_Resizable
                                            | ImGuiTableFlags_BordersOuter
                                            | ImGuiTableFlags_BordersV;


};



template <typename It, typename lambda_t>
void
EditorUI::selection_splitWindow( const It &begin, const It &end, const lambda_t &callback )
{
    ImGuiTableFlags flags  = ImGuiTableFlags_SizingStretchSame;
                    flags |= ImGuiTableFlags_Resizable;
                    flags |= ImGuiTableFlags_BordersOuter;
                    flags |= ImGuiTableFlags_BordersV;

    static int selected = 0;

    if (ImGui::BeginTable("table1", 2, flags))
    {
        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        for (auto iter = begin;  iter != end;  iter++)
        {
            int id = *(iter);
            std::string name = std::to_string(id);

            if (ImGui::Selectable(name.c_str(), selected == id))
            {
                selected = id;
            }
        }

        ImGui::TableNextColumn();
        callback(selected);

        ImGui::EndTable();
    }
}




template <typename T, typename lambda_t>
void
EditorUI::selection_splitWindow2( idk::Allocator<T> &container, const lambda_t &callback )
{
    ImGuiTableFlags flags  = ImGuiTableFlags_SizingStretchSame;
                    flags |= ImGuiTableFlags_Resizable;
                    flags |= ImGuiTableFlags_BordersOuter;
                    flags |= ImGuiTableFlags_BordersV;


    static T *selected_ptr = nullptr;
    int n = 0;

    if (ImGui::BeginTable("table1", 2, flags))
    {
        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        for (auto &item: container)
        {
            T *item_ptr = &item;
            std::string name = std::to_string(n);

            if (ImGui::Selectable(name.c_str(), selected_ptr == item_ptr))
            {
                selected_ptr = item_ptr;
            }

            n += 1;
        }

        ImGui::TableNextColumn();
        if (selected_ptr != nullptr)
        {
            callback(*selected_ptr);
        }

        ImGui::EndTable();
    }
}


