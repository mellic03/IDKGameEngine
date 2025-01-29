#pragma once

#include <IDKGameEngine/IDKengine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>

#include "../../external/include/idk_imgui/imgui.hpp"
#include "../../external/include/idk_icons/idk_Icons.hpp"

#include <deque>


namespace idk
{
    class EditorUI;
}


class idk::EditorUI: public idk::Module
{
private:
    idk::ECS* m_ecs = nullptr;

    bool m_demowindow = false;

    // Font
    // -----------------------------------------------------------------------------------------
    static constexpr int NUM_FONTS = 4;

    const std::array<float, NUM_FONTS> FONT_SIZES = { 14.0f, 16.0f, 18.0f, 20.0f };
    std::array<ImFont *, NUM_FONTS> m_fonts;

    ImFont*    m_font;
    ImFont*    m_icons;
    // -----------------------------------------------------------------------------------------


    using tabfn_type = std::function<void(idk::EngineAPI&)>;
    std::vector<tabfn_type> m_callbacks;


    // -----------------------------------------------------------------------------------------

    // Menu Bar
    // -----------------------------------------------------------------------------------------
    void        _menubar( idk::EngineAPI& );
    void        _menubar_settings( idk::EngineAPI& );
    // -----------------------------------------------------------------------------------------


    template <typename T>
    static void drawComponent( idk::EngineAPI&, idk::ECS&, int obj_id ) {  };

    // void registerDrawComponents();


public:

    virtual void        init    ( idk::EngineAPI& ) final;
    virtual void        deinit  () final;
    virtual void        registerECS( idk::EngineAPI&, idk::ECS& );

    virtual void        stage_A ( idk::EngineAPI& ) final {  };
    virtual void        stage_B ( idk::EngineAPI& ) final;
    virtual void        stage_C ( idk::EngineAPI& ) final {  };
    
    void insertImGui( const tabfn_type &fn ) { m_callbacks.push_back(fn); }
};



