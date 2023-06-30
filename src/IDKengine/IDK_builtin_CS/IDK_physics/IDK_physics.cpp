#include "IDK_physics.h"




void
Physics_CS::init( idk::Engine &engine )
{
    _default_shader = engine.rengine().glinterface().compileShaderProgram(
        "assets/shaders/", "gb_geom.vs", "gb_geom.fs"
    );

    _physicsworld.init();

    auto box1 = idk_physics::createBoxCollider(0, {0.0f, 20.0f, 0.0f}, {1.0f, 1.0f, 1.0f});
    _physicsworld.addBoxCollider(box1);
   
    auto box2 = idk_physics::createBoxCollider(1, {1.2f, 30.0f, 0.0f}, {1.0f, 0.25f, 1.0f});
    _physicsworld.addBoxCollider(box2);

   
    auto box3 = idk_physics::createBoxCollider(1, {0.8f, 33.0f, 0.0f}, {1.0f, 0.25f, 1.0f});
    _physicsworld.addBoxCollider(box3);

}


void
Physics_CS::stage_B( idk::Engine &engine )
{
    _physicsworld.step(engine.deltaTime());

    auto objects = _physicsworld.getObjects();

    for (btCollisionObject *obj: objects)
    {
        btTransform trans;
        btRigidBody * body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
            body->getMotionState()->getWorldTransform(trans);
        else
            trans = obj->getWorldTransform();

        auto scaling = ((btBoxShape *)obj->getCollisionShape())->getHalfExtentsWithMargin();

        glm::mat4 mmmmm;
        trans.getOpenGLMatrix(glm::value_ptr(mmmmm));
        idk::Transform transform(mmmmm);
        transform.scale(glm::vec3((float)scaling.getX(), (float)scaling.getY(), (float)scaling.getZ()));

        idk::RenderEngine &ren = engine.rengine();
        ren.drawModel(_default_shader, 2, transform);
    }
}



void
Physics_CS::onAssignment( int obj_id, idk::Engine &engine )
{

}


void
Physics_CS::onGameObjectCreation( int obj_id )
{
    // if (obj_id >= _velocities.size())
    //     _velocities.resize(obj_id+1, glm::vec3(0.0f));
    // else
    //     _velocities[obj_id] = glm::vec3(0.0f);
}


void
Physics_CS::onGameObjectDeletion( int obj_id )
{

}


void
Physics_CS::onGameObjectCopy( int src_obj_id, int dest_obj_id )
{

}


glm::vec3 &
Physics_CS::getVelocity( int obj_id )
{
    // return _velocities[obj_id];
}
