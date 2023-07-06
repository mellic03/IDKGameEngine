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

    const int TRANSFORM     = engine.registerCS<Transform_CS>("transform");
    const int MODEL         = engine.registerCS<Model_CS>("model");
    const int PLAYERCONTORL = engine.registerCS<PlayerControl_CS>("playercontrol");
    const int PHYSICS       = engine.registerCS<Physics_CS>("physics");
    const int UI_SYS        = engine.registerCS<Builtin_UI>("ui");
    const int GRABBABLE     = engine.registerCS<Grabbable_CS>("grabbable");
    const int POINTLIGHT    = engine.registerCS<PointLight_CS>("pointlight");
    const int SPOTLIGHT     = engine.registerCS<SpotLight_CS>("spotlight");

    auto &tCS = engine.getCS<Transform_CS>(TRANSFORM);
    auto &mCS = engine.getCS<Model_CS>(MODEL);
    auto &pCS = engine.getCS<Physics_CS>(PHYSICS);
    auto &pointCS = engine.getCS<PointLight_CS>(POINTLIGHT);
    auto &spotCS = engine.getCS<SpotLight_CS>(SPOTLIGHT);
    auto &grabCS = engine.getCS<Grabbable_CS>(GRABBABLE);

    mCS.addDependencies("transform");
    pCS.addDependencies("transform");
    grabCS.addDependencies("transform", "physics");


    idk::RenderEngine &ren = engine.rengine();
    ren.modelManager().loadTextures("assets/textures/");
    ren.getCamera().ylock(true);
    ren.getCamera().transform().translate(glm::vec3(0.0f, 0.0f, 20.0f));


    // int spotlight_id = engine.createGameObject();
    // engine.giveComponents(spotlight_id, TRANSFORM, SPOTLIGHT );
    // tCS.translate(spotlight_id, glm::vec3(-20.0f, 20.0f, 40.0f));

    int pointlight_obj = engine.createGameObject();
    engine.giveComponents(pointlight_obj, TRANSFORM, POINTLIGHT );
    tCS.translate(pointlight_obj, glm::vec3(-20.0f, 20.0f, 40.0f));

    demos::cube_physics(engine, TRANSFORM, MODEL, PHYSICS, GRABBABLE);
    // demos::school(engine, TRANSFORM, MODEL, PHYSICS, GRABBABLE);


    while (engine.running())
    {
        engine.beginFrame();

        // tCS.getTransform(spotlight_id).rotateX(-0.2f * engine.deltaTime());
        // tCS.getTransform(spotlight_id) = ren.getCamera().transform();// .rotateX(-0.2f * engine.deltaTime());
        // tCS.getTransform(spotlight_id).translate(-1.0f * ren.getCamera().front());
    
        engine.endFrame();
    }

    return 0;
}



