#include "../EditorUI.hpp"
#include "EditorUI-tabs.hpp"
#include "../idkui_components.hpp"


void
EditorUI_Module::_tab( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    this->_tab_scene_hierarchy(api);
    this->_tab_editor_properties(api);
    this->_tab_inspect(api, _get_selection());

    if (engine.hasComponent<idk::Transform_CS>(_get_selection()))
    {
        idkg::ui::transform_component(
            engine, ren.getCamera(), engine.getCS<idk::Transform_CS>(),
            _get_selection(), m_tsnap, m_rsnap
        );
    }

    EditorTab::shader_programs(api);
    EditorTab::skybox(api);
    EditorTab::cameras(api);
    EditorTab::models(api);

}
