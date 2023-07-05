#include "idk_physics.h"
#include "../idk_transform_CS.h"

#include <glm/gtx/matrix_decompose.hpp>



static glm::vec3 vec3_cast(btVector3 v)
{
    return glm::vec3(v.getX(), v.getY(), v.getZ());
}

static btVector3 btVec3_cast(glm::vec3 v)
{
    return btVector3(v.x, v.y, v.z);
}



void
Physics_CS::init( idk::Engine &engine )
{
    _engineptr = &engine;
    _physicsworld.init();
}


void
Physics_CS::stage_B( idk::Engine &engine )
{
    Transform_CS &tCS = _engineptr->getCS<Transform_CS>("transform");
 
    _physicsworld.step(engine.deltaTime());

    auto objects = _physicsworld.getObjects();

    for (int i=0; i<_rigidbodies.size(); i++)
    {
        if (_has_rigidbody[i])
        {
            auto scaling = ((btBoxShape *)_rigidbodies[i]->getCollisionShape())->getHalfExtentsWithMargin();
            btTransform trans = _rigidbodies[i]->getWorldTransform();

            glm::mat4 mmmmm;
            trans.getOpenGLMatrix(glm::value_ptr(mmmmm));

            tCS.getTransform(i) = idk::Transform(mmmmm);
            tCS.getTransform(i).scale(glm::vec3((float)scaling.getX(), (float)scaling.getY(), (float)scaling.getZ()));
        }

        else if (_has_mesh[i])
        {
            btTransform trans = _meshbodies[i]->getWorldTransform();
            glm::mat4 mmmmm;
            trans.getOpenGLMatrix(glm::value_ptr(mmmmm));
            tCS.getTransform(i) = idk::Transform(mmmmm);
        }
    }

}



void
Physics_CS::onAssignment( int obj_id, idk::Engine &engine )
{

}


void
Physics_CS::onGameObjectCreation( int obj_id )
{
    if (obj_id >= _has_rigidbody.size())
    {
        _has_mesh.resize(obj_id+1, false);
        _meshbodies.resize(obj_id+1);

        _has_rigidbody.resize(obj_id+1, false);
        _rigidbodies.resize(obj_id+1);
    }
    else
    {

    }
}


void
Physics_CS::onGameObjectDeletion( int obj_id )
{

}


void
Physics_CS::onGameObjectCopy( int src_obj_id, int dest_obj_id )
{
    // _has_rigidbody[dest_obj_id] = _has_rigidbody[src_obj_id];
    // _rigidbodies[dest_obj_id] = _rigidbodies[src_obj_id];
}


void
Physics_CS::giveBoxCollider( int obj_id, float mass, glm::vec3 size )
{
    Transform_CS &tCS = _engineptr->getCS<Transform_CS>("transform");

    idk::Transform transform = tCS.getTransform(obj_id);
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(transform.modelMatrix(), scale, rotation, translation, skew,perspective);

    idk_physics::BoxCollider box = idk_physics::createBoxCollider(
        mass, translation, size
    );

    btQuaternion quat(
        rotation.x,
        rotation.y,
        rotation.z,
        rotation.w
    );

    box.rigidbody->getWorldTransform().setRotation(quat);

    _physicsworld.addBoxCollider(box);
    _rigidbodies[obj_id] = box.rigidbody;
    _has_rigidbody[obj_id] = true;
}


void
Physics_CS::giveBoxCollider( int obj_id, float mass )
{
    Transform_CS &tCS = _engineptr->getCS<Transform_CS>("transform");

    idk::Transform transform = tCS.getTransform(obj_id);
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(transform.modelMatrix(), scale, rotation, translation, skew,perspective);

    idk_physics::BoxCollider box = idk_physics::createBoxCollider(
        mass, translation, scale
    );

    btQuaternion quat(
        rotation.x,
        rotation.y,
        rotation.z,
        rotation.w
    );

    box.rigidbody->getWorldTransform().setRotation(quat);

    _physicsworld.addBoxCollider(box);
    _rigidbodies[obj_id] = box.rigidbody;
    _has_rigidbody[obj_id] = true;
}


void
Physics_CS::giveBoxCollider( int obj_id, idk_physics::BoxColliderConfig &config )
{
    Transform_CS &tCS = _engineptr->getCS<Transform_CS>("transform");

    idk::Transform transform = tCS.getTransform(obj_id);
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(transform.modelMatrix(), scale, rotation, translation, skew,perspective);

    idk_physics::BoxCollider box = idk_physics::createBoxCollider(
        config
    );

    btQuaternion quat(
        rotation.x,
        rotation.y,
        rotation.z,
        rotation.w
    );

    box.rigidbody->getWorldTransform().setRotation(quat);

    _physicsworld.addBoxCollider(box);
    _rigidbodies[obj_id] = box.rigidbody;
    _has_rigidbody[obj_id] = true;
}


// void
// Physics_CS::giveMeshCollider( int obj_id, std::vector<idk::Vertex> &vertices )
// {
//     auto &tCS = _engineptr->getCS<Transform_CS>("transform");

//     btTriangleMesh *trimesh = new btTriangleMesh();

//     for (int i=0; i<vertices.size(); i+=3)
//     {
//         trimesh->addTriangle(
//             btVec3_cast(vertices[i+0].position),
//             btVec3_cast(vertices[i+1].position),
//             btVec3_cast(vertices[i+2].position)
//         );
//     }

//     /*
    
//         box.shape = new btBoxShape(btVector3(size.x, size.y, size.z));
//         box.motionstate = new btDefaultMotionState(translate(btVector3(pos.x, pos.y, pos.z)));
//         btVector3 local_inertia = {0, 0, 0};
//         box.shape->calculateLocalInertia(mass, local_inertia);
//         box.rigidbody = new btRigidBody(mass, box.motionstate, box.shape, local_inertia);

//     */

//     btCompoundShape *cshape = new btCompoundShape();

// 	btTransform	trans;
// 	trans.setIdentity();
// 	trans.setOrigin( btVec3_cast(tCS.getTransform(obj_id).position()) );


//     btCollisionShape *trishape = new btBvhTriangleMeshShape(trimesh, true);
// 	btDefaultMotionState* motionstate = new btDefaultMotionState( trans );
//     btVector3 inertia = btVector3(0.0, 1.0, 0.0);
// 	trishape->calculateLocalInertia( 1.0f, inertia );
// 	btRigidBody* body = new btRigidBody( 1.0f, motionstate, trishape, inertia );

//     _physicsworld._dynamicsWorld->addRigidBody(body);

//     _meshbodies[obj_id] = body;
//     _has_mesh[obj_id] = true;
// }
