#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

#include <iostream>
#include "IDKengine.h"
#include "demos/demos.h"


int ENTRY(int argc, const char **argv)
{
    idk::Engine engine(1920, 1080);
    const uint TRANSFORM     = engine.registerCS<Transform_CS>("transform");
    const uint MODEL         = engine.registerCS<Model_CS>("model");
    const uint PLAYERCONTORL = engine.registerCS<PlayerControl_CS>("playercontrol");
    const uint PHYSICS       = engine.registerCS<Physics_CS>("physics");
    const uint UI_SYS        = engine.registerCS<Builtin_UI>("ui");
    const uint GRABBABLE     = engine.registerCS<Grabbable_CS>("grabbable");

    auto &tCS = engine.getCS<Transform_CS>(TRANSFORM);
    auto &mCS = engine.getCS<Model_CS>(MODEL);
    auto &pCS = engine.getCS<Physics_CS>(PHYSICS);


    idk::RenderEngine &ren = engine.rengine();
    ren.modelManager().loadTextures("assets/textures/");

    int p1_id = ren.createPointLight();
    ren.pointlights().get(p1_id).transform.translate(glm::vec3(-20.0f, 20.0f, 40.0f));


    ren.getCamera().ylock(true);
    ren.getCamera().transform().translate(glm::vec3(0.0f, 0.0f, 20.0f));


    demos::cube_physics(engine, TRANSFORM, MODEL, PHYSICS, GRABBABLE);
    // demos::school(engine, TRANSFORM, MODEL, PHYSICS, GRABBABLE);


    while (engine.running())
    {
        engine.beginFrame();


        engine.endFrame();
    }

    return 0;
}



