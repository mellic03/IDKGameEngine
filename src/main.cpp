#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

#include <iostream>
#include "IDKengine.h"


enum class shader_instruction
{
    use_shader, set_uniform, draw_model
};


struct shader_element
{
    shader_instruction      instruction;
    void *                  data;
};




int ENTRY(int argc, const char **argv)
{
    idk::Engine engine;

    GLuint mouse_shader = engine.rengine().compileShaderProgram("assets/shaders/", "mouse.vs", "mouse.fs");
    uint model_id  = engine.rengine().loadOBJ("assets/models/cube/", "cube.obj", "cube.mtl");
    uint transform_id = engine.rengine().createTransform();

    float val = 0.0f;

    while (engine.running())
    {
        engine.beginFrame();

        engine.rengine().glInterface().bindShaderProgram(mouse_shader);
        engine.rengine().glInterface().setfloat("val", val);
        engine.rengine().glInterface().setvec2("mouse", glm::value_ptr(engine.mouse()));
        engine.rengine().drawModel(model_id, transform_id);

        engine.rengine().getTransform(transform_id).scale(glm::vec3(0.999f, 0.999f, 0.0f));

        val += 0.001f;

        engine.endFrame();
    }

    return 0;
}

