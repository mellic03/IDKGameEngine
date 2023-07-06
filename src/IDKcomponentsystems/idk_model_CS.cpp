#include "idk_model_CS.h"
#include "idk_transform_CS.h"


void
Model_CS::init(idk::Engine &engine)
{
    _default_shader = idk::glInterface::compileShaderProgram(
        "assets/shaders/", "gb_geom.vs", "gb_geom.fs"
    );

    addDependencies("transform");
}


void
Model_CS::stage_A(idk::Engine &engine)
{
    Transform_CS &tCS = engine.getCS<Transform_CS>("transform");

    idk::RenderEngine &ren = engine.rengine();

    std::vector<int> obj_ids = engine.gameObjects_byComponent(_component_index);
    for (int obj_id: obj_ids)
    {
        ren.drawModel(_default_shader, _model_ids[obj_id], tCS.getTransform(obj_id));
    }
}


void
Model_CS::onGameObjectCreation( int obj_id, idk::Engine &engine )
{
    if (obj_id >= _model_ids.size())
        _model_ids.resize(obj_id+1, -1);
    else
        _model_ids[obj_id] = -1;
}


void
Model_CS::onGameObjectDeletion( int obj_id, idk::Engine &engine )
{
    _model_ids[obj_id] = -1;
}


void
Model_CS::onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{
    _model_ids[dest_obj_id] = _model_ids[src_obj_id];
}
