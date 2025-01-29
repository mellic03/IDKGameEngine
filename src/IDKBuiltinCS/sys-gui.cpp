#include "sys-gui.hpp"


void
idk::GUISys::init( idk::EngineAPI &api )
{

}


void
idk::GUISys::update( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    auto &ecs = *m_ecs;

    glm::vec2 res = glm::vec2(ren.resolution());

    for (auto &cmp: ecs.getComponentArray<GUIButtonCmp>())
    {
        cmp.element->setCornerAbsolute(res/2.0f);
        cmp.element->setSpanAbsolute(glm::vec2(256.0f, 128.0f));
        cmp.element->updateLayout(glm::vec2(0.0f), res, 1);


    }
}


int
idk::GUISys::createButton( const std::string &label, const std::function<void()> &callback )
{
    auto &ecs = *m_ecs;
    int btn = ecs.createGameObject("ButtonTest", false);
    ecs.giveComponent<idk::GUIButtonCmp>(btn);
    ecs.getComponent<idk::GUIButtonCmp>(btn).element->m_label = label;
    return btn;
}


idkui::Button*
idk::GUISys::getButton( int obj )
{
    return m_ecs->getComponent<idk::GUIButtonCmp>(obj).element;
}



void
idk::GUISys::renderTexture( idk::EngineAPI &api, idkui::LayoutManager *LM )
{
    auto &ecs = *m_ecs;
    LM->clearTexture(api, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &cmp: ecs.getComponentArray<GUIButtonCmp>())
    {
        LM->updateInput(cmp.element);
        LM->renderNode(cmp.element);
    }

    LM->renderTexture(api);
}



size_t
idk::GUIButtonCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
}


size_t
idk::GUIButtonCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
}


void
idk::GUIButtonCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<GUIButtonCmp>(obj_id);
    cmp.element = new idkui::Button(cmp.label, cmp.style1, cmp.style2, [](){});
    cmp.element->m_absolute = true;
}


void
idk::GUIButtonCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    
}


void
idk::GUIButtonCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
{

}


