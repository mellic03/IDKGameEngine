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
    auto rendermod = engine.registerModule<idk::builtin_modules::Builtin_RenderPipeline>();
    auto controlmod = engine.registerModule<idk::builtin_modules::Builtin_PlayerControl>();
    auto physicsmod = engine.registerModule<idk::builtin_modules::Builtin_Physics>();
    auto uimod = engine.registerModule<idk::builtin_modules::Builtin_UI>();
    auto gmod = engine.registerModule<Grabbable>();
    auto tmod = engine.registerModule<transform_CS>();


    idk::RenderEngine &ren = engine.rengine();
    ren.loadTextures("assets/textures/");
    ren.loadSpherePrimitive("assets/models/uvsp.obj");


    uint light1 = ren.createPointLight();
    uint light2 = ren.createPointLight();
    uint light3 = ren.createPointLight();
    ren.pointlights().get(light1).diffuse = glm::vec3(0.3f, 0.3f, 1.0f);
    ren.pointlights().get(light2).diffuse = glm::vec3(1.0f, 0.3f, 0.3f);
    ren.pointlights().get(light3).diffuse = glm::vec3(0.3f, 1.0f, 0.3f);


    uint suz = ren.loadOBJ("assets/models/", "baby.obj", "baby.mtl");
    uint obj1 = engine.createGameObject();
    engine.giveComponent(obj1, 4);
    engine.getGameObject(obj1).model_id = suz;
    engine.translate(obj1, glm::vec3(0.0f, -33.5f, 0.0f));

    uint plane_id = ren.loadOBJ("assets/models/", "plane.obj", "plane.mtl");
    uint obj2 = engine.createGameObject();
    engine.getGameObject(obj2).model_id = plane_id;
    engine.translate(obj2, glm::vec3(0.0f, -1.5f, 0.0f));


    constexpr int spread = 100;
    for (int i=0; i<200; i++)
    {
        uint obj_id = engine.createGameObject(obj1);
        engine.translate(obj_id, glm::vec3((rand()%spread) - (spread/2), (rand()%spread), (rand()%spread) - (spread/2)));

        idk::Transform &t = ren.getTransform(engine.getGameObject(obj_id).transform_id);
        t.rotateX((rand()%628) / 100.0f);
        t.rotateY((rand()%628) / 100.0f);
        t.rotateZ((rand()%628) / 100.0f);
    }


    uint cam_id = ren.createCamera();
    ren.setActiveCamera(cam_id);
    ren.getActiveCamera().ylock(true);
    ren.getActiveCamera().noroll(true);

    float aaa =  0.0f;
    float bbb = +2.0f;
    float ccc = -2.0f;

    while (engine.running())
    {
        engine.beginFrame();

        auto &t1 = ren.getTransform(ren.pointlights().get(light1).transform_id);
        auto &t2 = ren.getTransform(ren.pointlights().get(light2).transform_id);
        auto &t3 = ren.getTransform(ren.pointlights().get(light3).transform_id);

        t1.position() = glm::vec3( 15*sin(aaa),  20.0f,  15*cos(aaa) );
        t2.position() = glm::vec3( 15*sin(bbb),  20.0f,  15*cos(bbb) );
        t3.position() = glm::vec3( 15*sin(ccc),  20.0f,  15*cos(ccc) );

        aaa += 2.0f * engine.deltaTime();
        bbb += 2.0f * engine.deltaTime();
        ccc += 2.0f * engine.deltaTime();

        engine.endFrame();
    }

    return 0;
}

