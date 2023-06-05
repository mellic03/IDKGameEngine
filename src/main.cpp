#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

#include <iostream>
#include "IDKengine.h"



int ENTRY(int argc, const char **argv)
{
    idk::Engine engine;

    int shader_id = engine.rengine().compileShaderProgram("assets/shaders/", "screenquad.vs", "mouse.fs");
    int model_id  = engine.rengine().loadOBJ("assets/models/", "cube.obj", "cube.mtl");
    int transform_id = engine.rengine().createTransform();

    while (engine.running())
    {
        engine.beginFrame();

        engine.rengine().useShaderProgram(shader_id);
        engine.rengine().drawModel(model_id, transform_id);


        engine.endFrame();
    }

    return 0;
}

