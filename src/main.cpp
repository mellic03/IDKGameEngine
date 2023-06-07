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


class Actor: public idk::GameObject::Renderable, public idk::GameObject::Physical
{
private:

public:
    Actor() {  };
};



int ENTRY(int argc, const char **argv)
{
    idk::Engine engine;
    idk::RenderEngine &ren = engine.rengine();
    idk::glInterface &gl = ren.glInterface();

    GLuint mouse_shader = gl.compileShaderProgram("assets/shaders/", "gb_geom.vs", "gb_geom.fs");
    uint model_id  = ren.loadOBJ("assets/models/cube/", "cube.obj", "cube.mtl");
    
    idk::transform &transform = ren.getTransform(ren.createTransform());
    transform.translate(glm::vec3(0.0f, 0.0f, 2.50f));

    // idk::transform &transform2 = ren.getTransform(ren.createTransform());
    // transform2.translate(glm::vec3(5.0f, 0.0f, 2.50f));


    ren.setActiveCamera(ren.createCamera());
    idk::Camera &camera = ren.getActiveCamera();

    while (engine.running())
    {
        engine.beginFrame();

        ren.bindShader(mouse_shader);
        ren.bindModel(model_id, transform);
        ren.setvec2("mouse", engine.mouse());

        engine.endFrame();
    }

    return 0;
}

