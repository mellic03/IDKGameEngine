#include "../EditorUI.hpp"
#include "EditorUI-tabs.hpp"
#include "../idkui_components.hpp"

#include <IDKEvents/IDKEvents.hpp>


void
EditorUI_MD::_tab( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    m_ecs = _tab_scene_select(api);

    if (m_ecs == nullptr)
    {
        m_ecs = &api.getScene();
    }

    auto &ecs = *m_ecs;

    _tab_scene_hierarchy(api, ecs);
    _tab_render_hierarchy(api);
    _tab_editor_properties(api);
    _tab_inspect(api, ecs, ecs.getSelectedGameObject());
    _tab_scripts(api);
    _tab_buffers(api);


    EditorTab::shader_programs(api);
    EditorTab::skybox(api);
    EditorTab::cameras(api);
    EditorTab::models(api);
    EditorTab::dirlights(api);


    // if (api.getEventSys().mouseCaptured())
    // {
    //     return;
    // }

    // static int lctrl_down = 0;
    // static int d_down     = 0;

    // if (api.getEventSys().keylog().keyTapped(idk::Keycode::LCTRL))
    // {
    //     lctrl_down = 64;
    // }

    // if (api.getEventSys().keylog().keyTapped(idk::Keycode::D))
    // {
    //     d_down = 64;
    // }

    // if (lctrl_down > 0) lctrl_down -= 1;
    // if (d_down > 0)     d_down -= 1;


    // if (lctrl_down && d_down)
    // {
    //     lctrl_down = 0;
    //     d_down     = 0;

    //     idk::ECS2::setSelectedGameObject(idk::ECS2::copyGameObject(idk::ECS2::getSelectedGameObject()));
    // }

}
