#include "IDKengine.h"


namespace demos
{


void school( idk::Engine &engine, int TRANS, int MOD, int PHYS, int GRAB )
{
    auto &tCS = engine.getCS<Transform_CS>(TRANS);
    auto &mCS = engine.getCS<Model_CS>(MOD);
    auto &pCS = engine.getCS<Physics_CS>(PHYS);

    idk::RenderEngine &ren = engine.rengine();

    int plane_id = ren.modelManager().loadOBJ("assets/models/", "rob.obj", "rob.mtl");
    int obj2 = engine.createGameObject();
    engine.giveComponents(obj2, TRANS, MOD);
    mCS.useModel(obj2, plane_id);

    std::cout << plane_id << "\n" << obj2 << std::endl;

}


};
