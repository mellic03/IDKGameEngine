#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

#include <iostream>
#include "IDKengine.h"


int ENTRY(int argc, const char **argv)
{
    idk::Engine engine(1920, 1080);
    const uint TRANSFORM     = engine.registerCS<Transform_CS>("transform");
    const uint MODEL         = engine.registerCS<Model_CS>("model");
    const uint PLAYERCONTORL = engine.registerCS<PlayerControl_CS>("playercontrol");
    const uint PHYSICS       = engine.registerCS<Physics_CS>("physics");
    const uint UI_SYS        = engine.registerCS<Builtin_UI>("ui");
    // const uint GRABBABLE     = engine.registerCS<Grabbable_CS>("grabbable");

    auto &mCS = engine.getCS<Model_CS>(MODEL);
    auto &tCS = engine.getCS<Transform_CS>(TRANSFORM);

    idk::RenderEngine &ren = engine.rengine();
    ren.loadTextures("assets/textures/");

    uint p1_id = ren.createPointLight();
    ren.pointlights().get(p1_id).transform.translate(glm::vec3(0.0f, 10.0f, 0.0f));

    uint suz = ren.loadOBJ("assets/models/", "cube.obj", "cube.mtl");
    uint obj1 = engine.createGameObject();
    engine.giveComponents(obj1, TRANSFORM, MODEL, PHYSICS);
    mCS.useModel(obj1, suz);

    uint plane_id = ren.loadOBJ("assets/models/", "rob.obj", "rob.mtl");
    uint obj2 = engine.createGameObject();
    engine.giveComponents(obj2, MODEL);
    mCS.useModel(obj2, plane_id);


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

