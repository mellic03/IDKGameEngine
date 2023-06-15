#include "IDK_render.h"



void
idk::builtin_modules::Builtin_Render::init(idk::Engine &engine)
{
    _mouse_shader = engine.rengine().glinterface().compileShaderProgram(
        "assets/shaders/", "gb_geom.vs", "gb_geom.fs"
    );
}


void
idk::builtin_modules::Builtin_Render::stage_B(idk::Engine &engine)
{
    idk::RenderEngine &ren = engine.rengine();
    ren.bindShader(_mouse_shader);

    engine.gameObjects().for_each(
        [&ren](idk::GameObject &obj)
        {
            if (obj.model_id == -1)
                return;

            ren.bindModel(obj.model_id, obj.transform_id);
        }
    );
}

