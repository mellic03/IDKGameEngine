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
    engine.registerModule<idk::builtin_modules::Builtin_RenderPipeline>();
    engine.registerModule<idk::builtin_modules::Builtin_PlayerControl>();
    engine.registerModule<idk::builtin_modules::Builtin_Physics>();
    engine.registerModule<idk::builtin_modules::Builtin_UI>();

    idk::RenderEngine &ren = engine.rengine();
    ren.loadTextures("assets/textures/");

    idk::lightsource::Point plight = ren.createPointLight();
    ren.getTransform(plight.transform_id).translate(glm::vec3(0.0f, 2.0f, 0.0f));

    uint suz_id = ren.loadOBJ("assets/models/dog/", "dog.obj", "dog.mtl");
    uint obj1 = engine.createGameObject();
    engine.getGameObject(obj1).model_id = suz_id;

    uint plane_id = ren.loadOBJ("assets/models/", "plane.obj", "plane.mtl");
    uint obj2 = engine.createGameObject();
    engine.getGameObject(obj2).model_id = plane_id;


    uint pid1 = engine.createPrefab(obj1);
    uint obj3 = engine.createGameObject(pid1);
    ren.getTransform(engine.getGameObject(obj3).transform_id).translate(glm::vec3(0.0f, 2.0f, -5.0f));


    uint cam_id = ren.createCamera();
    ren.setActiveCamera(cam_id);
    ren.getActiveCamera().ylock(true);

    while (engine.running())
    {
        engine.beginFrame();



        engine.endFrame();
    }

    return 0;
}

