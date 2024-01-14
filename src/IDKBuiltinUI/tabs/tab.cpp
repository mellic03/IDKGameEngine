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


    // int selected_obj = _get_selection();

    // if (engine.hasComponent<idk::Transform_CS>(selected_obj))
    // {
    //     auto &tCS = engine.getCS<idk::Transform_CS>();

    //     glm::mat4 &model    = tCS.getModelMatrix(selected_obj);
    //     glm::mat4 transform = tCS.getModelMatrixParented(selected_obj);
    //     glm::mat4 parent    = tCS.getParentModelMatrix(selected_obj);

    //     idk::ui::transform_component(
    //         engine, ren.getCamera(), engine.getCS<idk::Transform_CS>(),
    //         model, transform, parent, m_tsnap, m_rsnap
    //     );
    // }

    EditorTab::shader_programs(api);
    EditorTab::skybox(api);
    EditorTab::cameras(api);
    EditorTab::models(api);

}
