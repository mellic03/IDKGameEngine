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
    float       m_tsnap = 0.0f;
    float       m_rsnap = 1.0f;
    float       m_ssnap = 0.1f;

    idk::ECS*   m_ecs = nullptr;

    using tabfn_type = std::function<void(idk::EngineAPI&)>;
    std::vector<tabfn_type> m_tabs;

    void        _tab_viewport( idk::EngineAPI&, idk::ECS& );
    idk::ECS*   _tab_scene_select( idk::EngineAPI& );
    void        _tab_scene_treenode( idk::EngineAPI&, idk::ECS&, int );
    void        _tab_scene_hierarchy( idk::EngineAPI&, idk::ECS& );
    void        _tab_render_hierarchy( idk::EngineAPI& );
    void        _tab_inspect( idk::EngineAPI&, idk::ECS&, int object_id );
    void        _tab_editor_properties( idk::EngineAPI& );
    void        _tab_scripts( idk::EngineAPI& );
    void        _tab_assets( idk::EngineAPI& );
    void        _tab_buffers( idk::EngineAPI& );
    void        _tab( idk::EngineAPI& );
    // -----------------------------------------------------------------------------------------

    // Menu Bar
    // -----------------------------------------------------------------------------------------
    void        _menubar( idk::EngineAPI& );
    void        _menubar_settings( idk::EngineAPI& );
    // -----------------------------------------------------------------------------------------


    template <typename T>
    static void drawComponent( idk::EngineAPI&, idk::ECS&, int obj_id ) {  };

    void registerDrawComponents();
    void clearDrawComponents();


public:
    virtual void        init    ( idk::EngineAPI& ) final;
    virtual void        deinit  () final;

    virtual void        stage_A ( idk::EngineAPI& ) final {  };
    virtual void        stage_B ( idk::EngineAPI& ) final;
    virtual void        stage_C ( idk::EngineAPI& ) final {  };

    void insertImGui( const tabfn_type &fn ) { m_tabs.push_back(fn); }
};



