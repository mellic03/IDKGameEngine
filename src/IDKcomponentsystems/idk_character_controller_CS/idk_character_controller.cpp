#include "idk_character_controller.h"
#include "../idk_physics_CS/idk_physics_CS.h"


void
CharacterController_CS::controlMethod(int obj_id, std::function<void(int, idk::Engine &)> lambda)
{
    int controller_id = _controller_ids[obj_id];
    _controllers.get(controller_id).controlMethod = lambda;
}


void
CharacterController_CS::init( idk::Engine &engine )
{
    addDependencies("physics");
}


void
CharacterController_CS::onAssignment( int obj_id, idk::Engine &engine )
{
    auto &pCS = engine.getCS<Physics_CS>("physics");
    pCS.giveCapsuleCollider(obj_id);

    CharacterController controller;
    controller.obj_id = obj_id;
    _controller_ids[obj_id] = _controllers.add(controller);
}


void
CharacterController_CS::stage_B( idk::Engine &engine )
{
    _controllers.for_each(
        [&engine](CharacterController &controller)
        {
            controller.controlMethod(controller.obj_id, engine);
        }
    );
}


void
CharacterController_CS::onGameObjectCreation( int obj_id, idk::Engine &engine )
{
    if (obj_id >= _controller_ids.size())
    {
        _controller_ids.resize(obj_id+1, -1);
    }

    else
    {
        _controller_ids[obj_id] = -1;
    }
}


void
CharacterController_CS::onGameObjectDeletion( int obj_id, idk::Engine &engine )
{
    _controllers.remove(_controller_ids[obj_id]);
    _controller_ids[obj_id] = -1;
}


void
CharacterController_CS::onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{
    _controller_ids[dest_obj_id] = _controllers.add();
}
