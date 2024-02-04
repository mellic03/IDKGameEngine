#pragma once

#include <IDKGameEngine/IDKengine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>

#include "../../external/include/idk_imgui/imgui.hpp"
#include "../../external/include/idk_icons/idk_Icons.hpp"

#include <deque>


class EditorUI_MD: public idk::Module
{
private:
    static constexpr int NUM_FONTS = 4;

    const std::array<float, NUM_FONTS> FONT_SIZES = { 14.0f, 16.0f, 18.0f, 20.0f };
    std::array<ImFont *, NUM_FONTS> m_fonts;

    ImFont *    m_font;
    ImFont *    m_icons;

    bool        m_show_ImGui_demo = false;

    // Editor Tabs
    // -----------------------------------------------------------------------------------------
    float       m_tsnap = 0.50f;
    float       m_rsnap = 45.0f;

    void        _tab_viewport( idk::EngineAPI & );

    void        _tab_scene_treenode( idk::EngineAPI &, int );
    void        _tab_scene_hierarchy( idk::EngineAPI & );
    void        _tab_inspect( idk::EngineAPI &, int object_id );
    void        _tab_editor_properties( idk::EngineAPI & );
    void        _tab_scripts( idk::EngineAPI & );
    void        _tab( idk::EngineAPI & );
    // -----------------------------------------------------------------------------------------

    // Menu Bar
    // -----------------------------------------------------------------------------------------
    void        _menubar( idk::EngineAPI & );
    void        _menubar_settings( idk::EngineAPI & );
    // -----------------------------------------------------------------------------------------

    template <typename T>
    void drawComponent( idk::EngineAPI &, int obj_id ) {  };


public:

    virtual void        init    ( idk::EngineAPI & ) final;
    virtual void        deinit  () final;
    virtual void        stage_A ( idk::EngineAPI & ) final {  };
    virtual void        stage_B ( idk::EngineAPI & ) final;
    virtual void        stage_C ( idk::EngineAPI & ) final {  };

};



