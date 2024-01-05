#include "idk_model_cs.hpp"
#include "idk_transform_cs.hpp"


void
idk::Model_CS::init( idk::EngineAPI &api )
{
    addDependencies("transform");
}


void
idk::Model_CS::stage_A( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    Transform_CS &tCS = engine.getCS<Transform_CS>();

    for (auto &[obj_id, model_id, animator_id]: m_object_model_ids)
    {
        glm::mat4 transform = tCS.getModelMatrixParented(obj_id);
        ren.drawModel(model_id, animator_id, transform);

        if (m_shadowcasts[obj_id])
        {
            ren.drawShadowCaster(model_id, animator_id, transform);
        }
    }

}


void
idk::Model_CS::useModel( idk::RenderEngine &ren, int obj_id, int model_id )
{
    int animator_id = ren.modelSystem().getModel(model_id).animator_id;

    m_keys[obj_id] = m_object_model_ids.create({obj_id, model_id, animator_id});
}


int
idk::Model_CS::getModelID( int obj_id )
{
    int key      = m_keys[obj_id];
    int model_id = std::get<1>(m_object_model_ids.get(key));

    return model_id;
}


idk::Model &
idk::Model_CS::getModel( idk::RenderEngine &ren, int obj_id )
{
    int model_id = getModelID(obj_id);
    return ren.modelSystem().getModel(model_id);
}


int
idk::Model_CS::getAnimatorID( idk::RenderEngine &ren, int obj_id )
{
    int model_id = getModelID(obj_id);
    return ren.modelSystem().getAnimatorID(model_id);
}


idk::Animator &
idk::Model_CS::getAnimator( idk::RenderEngine &ren, int obj_id )
{
    int animator_id = getAnimatorID(ren, obj_id);
    return ren.modelSystem().getAnimator(animator_id);
}



void
idk::Model_CS::onObjectDeletion( int obj_id, idk::Engine &engine )
{
    std::cout << "Deleting object " << obj_id << " with Model_CS component\n";

    int &key = m_keys[obj_id];
    m_object_model_ids.destroy(key);
    key = -1;
}



void
idk::Model_CS::onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{
    engine.giveComponent(dest_obj_id, this->m_id);

    int key      = m_keys[src_obj_id];
    int model_id = std::get<1>(m_object_model_ids.get(key));

    m_keys[dest_obj_id] = m_object_model_ids.create({dest_obj_id, model_id, -1});
}


