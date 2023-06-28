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
    const uint GRABBABLE     = engine.registerCS<Grabbable_CS>("grabbable");
    const uint SPHERECOL     = engine.registerCS<SphereCollider_CS>("spherecollider");
    const uint PLIGHT        = engine.registerCS<Lightsource_CS>("pointlight");

    Transform_CS *tCS = engine.getCS<Transform_CS>("transform");
    Model_CS *mCS = engine.getCS<Model_CS>("model");


    idk::RenderEngine &ren = engine.rengine();
    ren.loadTextures("assets/textures/");
    ren.loadSpherePrimitive("assets/models/uvsp.obj");

    engine.giveComponents(engine.createGameObject(), TRANSFORM, PHYSICS, GRABBABLE, PLIGHT);


    uint suz = ren.loadOBJ("assets/models/", "dog.obj", "dog.mtl");
    uint obj1 = engine.createGameObject();
    engine.giveComponents(obj1, TRANSFORM, PHYSICS, GRABBABLE, SPHERECOL, MODEL);
    mCS->useModel(obj1, suz);
    engine.getCS<SphereCollider_CS>(SPHERECOL)->setRadius(obj1, 1.7f);


    uint plane_id = ren.loadOBJ("assets/models/", "rob.obj", "rob.mtl");
    uint obj2 = engine.createGameObject();
    engine.giveComponents(obj2, TRANSFORM, MODEL);
    mCS->useModel(obj2, plane_id);
    tCS->translate(obj2, glm::vec3(0.0f, -1.11f, 0.0f));


    constexpr int spread = 100;
    for (int i=0; i<200; i++)
    {
        uint obj_id = engine.createGameObject(obj1);
        tCS->translate(
            obj_id,
            glm::vec3(
                (rand()%spread) - (spread/2),
                (rand()%spread),
                (rand()%spread) - (spread/2)
            )
        );

        idk::Transform &t = tCS->getTransform(obj_id);
        t.rotateX((rand()%628) / 100.0f);
        t.rotateY((rand()%628) / 100.0f);
        t.rotateZ((rand()%628) / 100.0f);
    }
    engine.deleteGameObject(obj1);


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

