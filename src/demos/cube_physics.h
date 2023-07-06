#include "IDKengine.h"


namespace demos
{


void cube_physics( idk::Engine &engine, int TRANS, int MOD, int PHYS, int GRAB )
{
    auto &tCS = engine.getCS<Transform_CS>(TRANS);
    auto &mCS = engine.getCS<Model_CS>(MOD);
    auto &pCS = engine.getCS<Physics_CS>(PHYS);

    idk::RenderEngine &ren = engine.rengine();

    int cube = ren.modelManager().loadOBJ("assets/models/", "cube.obj", "cube.mtl");
    int stoneplane = ren.modelManager().loadOBJ("assets/models/", "stoneplane.obj", "stoneplane.mtl");
    
    int obj1 = engine.createGameObject();
    engine.giveComponents(obj1, TRANS, MOD, PHYS);
    mCS.useModel(obj1, cube);
    tCS.getTransform(obj1).scale(glm::vec3(10.0f, 1.0f, 10.0f));
    pCS.giveBoxCollider(obj1, 0.0f);


    obj1 = engine.createGameObject();
    engine.giveComponents(obj1, TRANS, MOD, PHYS);
    mCS.useModel(obj1, stoneplane);
    tCS.getTransform(obj1).scale(glm::vec3(40.0f, 1.0f, 40.0f));
    tCS.getTransform(obj1).translate(glm::vec3(0.0f, -10.0f, 0.0f));
    pCS.giveBoxCollider(obj1, 0.0f);

    for (int i=0; i<10; i++)
    {
        for (int j=0; j<10; j++)
        {
            for (int k=0; k<10; k++)
            {
                int cube2 = engine.createGameObject();
                engine.giveComponents(cube2, TRANS, MOD, PHYS, GRAB);
                mCS.useModel(cube2, cube);

                idk::Transform &transform = tCS.getTransform(cube2);

                transform.translate(glm::vec3(i-5, 1.1+j, k-5));
                transform.scale(glm::vec3(0.45f));

                pCS.giveBoxCollider(cube2, 10.0f);
            }
        }
    }
}



};
