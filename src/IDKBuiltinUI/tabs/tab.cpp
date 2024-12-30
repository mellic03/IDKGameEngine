#include "../EditorUI.hpp"
#include "EditorUI-tabs.hpp"
#include "../idkui_components.hpp"

#include <IDKEvents/IDKEvents.hpp>


void
EditorUI_MD::_tab( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    
    auto &ren    = api.getRenderer();

    this->_tab_scene_hierarchy(api);
    this->_tab_render_hierarchy(api);
    this->_tab_editor_properties(api);
    this->_tab_inspect(api, idk::ECS2::getSelectedGameObject());
    this->_tab_scripts(api);
    this->_tab_buffers(api);


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
