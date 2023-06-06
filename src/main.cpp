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

    GLuint shader_id = engine.rengine().compileShaderProgram("assets/shaders/", "screenquad.vs", "mouse.fs");
    uint model_id  = engine.rengine().loadOBJ("assets/models/cube/", "cube.obj", "cube.mtl");
    uint transform_id = engine.rengine().createTransform();


    while (engine.running())
    {
        engine.beginFrame();

        engine.rengine().useShaderProgram(shader_id);
        engine.rengine().activeShader().setvec2("mouse", engine.mouse());

        engine.rengine().drawModel(model_id, transform_id);
        engine.rengine().getTransform(transform_id).translate(glm::vec3(0.00001f, 0.0f, 0.0f));

        engine.endFrame();
    }

    return 0;
}

