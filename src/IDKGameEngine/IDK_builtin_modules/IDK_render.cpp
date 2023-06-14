#include "IDK_render.h"


void
idk::builtin_modules::Builtin_Render::stage_B(idk::Engine &engine)
{
    engine.gameObjects().for_each(
        [](idk::GameObject &obj)
        {
            if (obj.model_id == -1)
                return;

            std::cout << obj.model_id << std::endl;            
        }
    );
}

