#include "IDK_model_CS.h"
#include "IDK_transform_CS.h"


void
Model_CS::init(idk::Engine &engine)
{
    _default_shader = engine.rengine().glinterface().compileShaderProgram(
        "assets/shaders/", "gb_geom.vs", "gb_geom.fs"
    );
}


void
Model_CS::stage_A(idk::Engine &engine)
{
    Transform_CS &tCS = engine.getCS<Transform_CS>("transform");

    idk::RenderEngine &ren = engine.rengine();
    ren.bindShader(_default_shader);

    std::vector<int> obj_ids = engine.gameObjects_byComponent(_component_index);
    for (int obj_id: obj_ids)
    {
        ren.drawModel(_model_ids[obj_id], tCS.getTransform(obj_id));
    }
}


void
Model_CS::onGameObjectCreation( int obj_id )
{
    if (obj_id >= _model_ids.size())
        _model_ids.resize(obj_id+1, -1);
    else
        _model_ids[obj_id] = -1;
}


void
Model_CS::onGameObjectDeletion( int obj_id )
{
    _model_ids[obj_id] = -1;
}


void
Model_CS::onGameObjectCopy( int src_obj_id, int dest_obj_id )
{
    _model_ids[dest_obj_id] = _model_ids[src_obj_id];
}
