#include "IDK_render.h"



void
Builtin_RenderPipeline::init(idk::Engine &engine)
{
    _default_shader = engine.rengine().glinterface().compileShaderProgram(
        "assets/shaders/", "gb_geom.vs", "gb_geom.fs"
    );
}


void
Builtin_RenderPipeline::stage_A(idk::Engine &engine)
{
    idk::RenderEngine &ren = engine.rengine();
    ren.bindShader(_default_shader);

    engine.gameObjects().for_each(
        [&ren](idk::GameObject &obj)
        {
            if (obj.model_id == -1)
                return;

            ren.bindModel(obj.model_id, obj.transform_id);
        }
    );
}


