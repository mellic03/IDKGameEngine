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

    GLuint default_shader = idk::glInterface::compileProgram("assets/shaders/", "gb_geom.vs", "gb_geom.fs");
    GLuint skydome_shader = idk::glInterface::compileProgram("assets/shaders/", "gb_geom.vs", "skydome.fs");

    int player_obj = engine.createGameObject();
    engine.giveComponents(player_obj, TRANSFORM, PHYSICS, CAMERA, PLAYERCONTROL);
    transCS.translate(player_obj, glm::vec3(0.0f, 20.0f, 0.0f));
    physCS.giveCapsuleCollider(player_obj);
    // idk::lightsource::Point &pointlight = pointCS.getPointlight(player_obj);
    // pointlight.attenuation = glm::vec4(0.0f, 1.0f, 1.0f, 0.0f);

    int skydome_obj = engine.createGameObject();
    int skydome_model = ren.modelManager().loadOBJ("assets/models/", "skydome.obj", "skydome.mtl");
    engine.giveComponents(skydome_obj, TRANSFORM, MODEL);
    modelCS.useModel(skydome_obj, skydome_model, skydome_shader);
    transCS.getTransform(skydome_obj).scale(glm::vec3(120.0f));

    int terrain_obj = engine.createGameObject();
    int terrain_model = ren.modelManager().loadOBJ("assets/models/", "tree.obj", "tree.mtl");
    engine.giveComponents(terrain_obj, TRANSFORM, MODEL, PHYSICS);
    physCS.giveMeshCollider(terrain_obj, ren.modelManager().getModel(terrain_model).vertices);
    physCS.drawMeshColliders(true);
    modelCS.useModel(terrain_obj, terrain_model, default_shader);

    // demos::cube_physics(engine, TRANSFORM, MODEL, PHYSICS, GRABBABLE);
    // demos::school(engine, TRANSFORM, MODEL, PHYSICS, GRABBABLE);

    // int pointlight_obj = engine.createGameObject();
    // engine.giveComponents(pointlight_obj, TRANSFORM, POINTLIGHT );
    // transCS.translate(pointlight_obj, glm::vec3(10.0f, 15.0f, 0.0f));


    int spotlight_obj = engine.createGameObject();
    engine.giveComponents(spotlight_obj, TRANSFORM, SPOTLIGHT);
    glm::vec3 last_dir = ren.getCamera().front();



    while (engine.running())
    {
        engine.beginFrame();

        auto &transform = transCS.getTransform(spotlight_obj);
        transform = idk::Transform(glm::inverse(ren.getCamera().view()));
        transform.localTranslate(glm::vec3(0.0f, 0.0f, 1.0f));

        glm::vec3 front = glm::mat3(ren.getCamera().transform().modelMatrix()) * glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 dir = (front - last_dir);
        last_dir += 10.0f * engine.deltaTime() * dir;
        
        spotCS.getSpotlight(spotlight_obj).direction = glm::vec4(last_dir, 0.0f);

        engine.endFrame();
    }

    return 0;
}

