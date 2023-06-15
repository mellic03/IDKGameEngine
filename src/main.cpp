#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

#include <iostream>
#include "IDKengine.h"


int ENTRY(int argc, const char **argv)
{
    idk::Engine engine(2000, 1200);
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
    engine.translate(obj2, glm::vec3(0.0f, -3.0f, 0.0f));

    constexpr int dddd = 100;
    for (int i=0; i<1000; i++)
    {
        uint obj_id = engine.createGameObject(obj1);
        engine.translate(obj_id, glm::vec3((rand()%dddd) - (dddd/2), (rand()%dddd) - (dddd/2), (rand()%dddd) - (dddd/2)));
        engine.rengine().getTransform(engine.getGameObject(obj_id).transform_id).rotateX((rand()%628) / 100.0f);
        engine.rengine().getTransform(engine.getGameObject(obj_id).transform_id).rotateY((rand()%628) / 100.0f);
        engine.rengine().getTransform(engine.getGameObject(obj_id).transform_id).rotateZ((rand()%628) / 100.0f);
    }


    uint cam_id = ren.createCamera();
    ren.setActiveCamera(cam_id);
    ren.getActiveCamera().ylock(true);

    while (engine.running())
    {
        engine.beginFrame();


        engine.endFrame();
    
        std::cout << engine.frameRate() << std::endl;
    }

    return 0;
}

