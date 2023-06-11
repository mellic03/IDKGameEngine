#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

#include <iostream>
#include "IDKengine.h"


int ENTRY(int argc, const char **argv)
{
    idk::Engine engine(1500, 1000);
    engine.registerModule<idk::builtin_modules::Builtin_Physics>();
    engine.registerModule<idk::builtin_modules::Builtin_PlayerControl>();
    engine.registerModule<idk::builtin_modules::Builtin_UI>();

    idk::RenderEngine &ren = engine.rengine();
    idk::glInterface &gl = ren.glinterface();

    GLuint mouse_shader = gl.compileShaderProgram("assets/shaders/", "gb_geom.vs", "gb_geom.fs");

    uint model_id = ren.loadOBJ("assets/models/dog/", "dog.obj", "dog.mtl");
    uint cube_id = ren.loadOBJ("assets/models/cube/", "cube.obj", "cube.mtl");
    uint cubes_id = ren.loadOBJ("assets/models/", "cubes.obj", "cubes.mtl");
    uint plane_id = ren.loadOBJ("assets/models/", "plane.obj", "plane.mtl");

    uint t1_id = ren.createTransform();
    uint t2_id = ren.createTransform();
    uint t3_id = ren.createTransform();
    uint t4_id = ren.createTransform();

    ren.getTransform(t1_id).translate(glm::vec3(0.0f, 0.0f, -6.0f));
    ren.getTransform(t1_id).rotateY(2.0);
    ren.getTransform(t2_id).translate(glm::vec3(-4.0f, 0.0f, 0.0f));
    ren.getTransform(t2_id).rotateY(1.0);
    ren.getTransform(t3_id).translate(glm::vec3(0.0f, -1.0f, 0.0f));
    ren.getTransform(t3_id).rotateX(3.1415);
    ren.getTransform(t4_id).translate(glm::vec3(0.0f, -1.3f, 0.0f));

    idk::lightsource::Point plight = ren.createPointLight();
    ren.getTransform(plight.transform_id).translate(glm::vec3(0.0f, 2.0f, 0.0f));

    uint cam_id = ren.createCamera();
    ren.setActiveCamera(cam_id);
    ren.getActiveCamera().ylock(true);

    while (engine.running())
    {
        engine.beginFrame();

        ren.bindShader(mouse_shader);
        ren.bindModel(model_id, t1_id);
        ren.bindModel(cube_id,  t2_id);
        ren.bindModel(cubes_id, t4_id);
        ren.bindModel(plane_id, t3_id);

        float dtime = engine.deltaTime();
        ren.getTransform(t1_id).rotateY(0.55f * dtime);
        ren.getTransform(t2_id).rotateY(0.55f * dtime);

        engine.endFrame();
    }

    return 0;
}

