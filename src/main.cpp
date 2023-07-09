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
    idk::Engine engine("IDK Game", 1920, 1080);

    const int TRANSFORM     = engine.registerCS<Transform_CS>("transform");
    const int MODEL         = engine.registerCS<Model_CS>("model");
    const int PLAYERCONTROL = engine.registerCS<PlayerControl_CS>("playercontrol");
    const int PHYSICS       = engine.registerCS<Physics_CS>("physics");
    const int GRABBABLE     = engine.registerCS<Grabbable_CS>("grabbable");
    const int POINTLIGHT    = engine.registerCS<PointLight_CS>("pointlight");
    const int SPOTLIGHT     = engine.registerCS<SpotLight_CS>("spotlight");
    const int CAMERA        = engine.registerCS<Camera_CS>("camera");

    auto &transCS = engine.getCS<Transform_CS>(TRANSFORM);
    auto &modelCS = engine.getCS<Model_CS>(MODEL);
    auto &physCS = engine.getCS<Physics_CS>(PHYSICS);
    auto &pointCS = engine.getCS<PointLight_CS>(POINTLIGHT);
    auto &spotCS = engine.getCS<SpotLight_CS>(SPOTLIGHT);
    auto &grabCS = engine.getCS<Grabbable_CS>(GRABBABLE);

    idk::RenderEngine &ren = engine.rengine();
    ren.modelManager().loadTextures("assets/textures/");
    ren.getCamera().ylock(true);
    ren.getCamera().transform().translate(glm::vec3(0.0f, 0.0f, 20.0f));

    int spotlight_obj = engine.createGameObject();
    engine.giveComponents(spotlight_obj, TRANSFORM, SPOTLIGHT );
    tCS.translate(spotlight_obj, glm::vec3(-20.0f, 20.0f, 40.0f));

    int spotlight_obj = engine.createGameObject();
    engine.giveComponents(spotlight_obj, TRANSFORM, SPOTLIGHT );
    transCS.translate(spotlight_obj, glm::vec3(-20.0f, 20.0f, 40.0f));

    // demos::cube_physics(engine, TRANSFORM, MODEL, PHYSICS, GRABBABLE);
    demos::school(engine, TRANSFORM, MODEL, PHYSICS, GRABBABLE);

    while (engine.running())
    {
        engine.beginFrame();

        auto &transform = tCS.getTransform(spotlight_obj);
        transform = ren.getCamera().transform();
        transform.translate(-1.0f * ren.getCamera().front());
    
        engine.endFrame();
    }

    return 0;
}
