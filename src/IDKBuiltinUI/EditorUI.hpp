#pragma once

#include <IDKGameEngine/IDKengine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>

#include "../../external/include/idk_imgui/imgui.hpp"
#include "../../external/include/idk_icons/idk_Icons.hpp"

#include <deque>


class EditorUI_Module: public idk::Module
{
private:
    static constexpr int NUM_FONTS = 4;

    const std::array<float, NUM_FONTS> FONT_SIZES = { 14.0f, 16.0f, 18.0f, 20.0f };
    std::array<ImFont *, NUM_FONTS> m_fonts;

    ImFont *    m_font;
    ImFont *    m_icons;

    bool        m_show_ImGui_demo = false;

    // // Editor Tabs
    // // -----------------------------------------------------------------------------------------
    static constexpr int MAX_SELECTION_HISTORY = 10;
    std::deque<int> m_selection;

    float       m_tsnap = 0.25f;
    float       m_rsnap = 0.25f;

    void _select_object( int id )
    {
        m_selection.push_front(id);
    
        if (m_selection.size() > MAX_SELECTION_HISTORY)
        {
            m_selection.pop_back();
        }
    };
    int         _get_selection() { return m_selection.empty() ? 0 : m_selection.front(); };

    void        _tab_scene_treenode( idk::EngineAPI &, int );
    void        _tab_scene_hierarchy( idk::EngineAPI & );
    void        _tab_inspect( idk::EngineAPI &, int object_id );
    void        _tab_editor_properties( idk::EngineAPI & );
    void        _tab( idk::EngineAPI & );
    // // -----------------------------------------------------------------------------------------

    // // Menu Bar
    // // -----------------------------------------------------------------------------------------
    void        _menubar( idk::EngineAPI & );
    void        _menubar_settings( idk::EngineAPI & );
    // // -----------------------------------------------------------------------------------------


public:

    void        init( idk::EngineAPI & );
    void        stage_A( idk::EngineAPI & ) {  };
    void        stage_B( idk::EngineAPI & );
    void        stage_C( idk::EngineAPI & ) {  };

};

