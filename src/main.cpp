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
    idk::RenderEngine &ren = engine.rengine();
    idk::glInterface &gl = ren.glInterface();

    GLuint mouse_shader = gl.compileShaderProgram("assets/shaders/", "gb_geom.vs", "gb_geom.fs");
    uint model_id  = ren.loadOBJ("assets/models/dog/", "dog.obj", "dog.mtl");

    uint tid1 = ren.createTransform();
    idk::transform &transform = ren.getTransform(tid1);
    transform.translate(glm::vec3(0.0f, 0.0f, 3.50f));
    transform.rotateY(3.1415);

    uint pID = ren.createPointLight();
    idk::lightsource::Point &point = ren.pointlights().get(pID);
    ren.getTransform(point.transform_id).position() = glm::vec3(-5.0f, 3.0f, 2.0f);

    ren.setActiveCamera(ren.createCamera());
    idk::Camera &camera = ren.getActiveCamera();

    while (engine.running())
    {
        engine.beginFrame();

        ren.bindShader(mouse_shader);
        ren.bindModel(model_id, tid1);

        float dtime = ren.deltaTime();

        // ren.getTransform(tid1).rotateX(1.0f * dtime);
        ren.getTransform(tid1).rotateY(2.0f * dtime);

        engine.endFrame();
    }

    return 0;
}

