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

    engine.registerModule<idk::PhysicsModule>();

    idk::RenderEngine &ren = engine.rengine();
    idk::glInterface &gl = ren.glinterface();

    GLuint mouse_shader = gl.compileShaderProgram("assets/shaders/", "gb_geom.vs", "gb_geom.fs");
    uint model_id  = ren.loadOBJ("assets/models/dog/", "dog.obj", "dog.mtl");

    engine.createGameObject();
    engine.createGameObject();


    while (engine.running())
    {
        engine.beginFrame();



        engine.endFrame();
    }

    return 0;
}

