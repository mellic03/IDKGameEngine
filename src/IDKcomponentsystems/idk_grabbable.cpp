#include "idk_grabbable.h"
#include "idk_physics/idk_physics.h"
#include "idk_transform_CS.h"


constexpr float GRAB_DIST = 3.0f;
constexpr float GRAB_DIST_SQ = 9.0f;
constexpr float THROW_SPEED = 5000.0f;

static int grabbed_id = -1;


#define SQ(a) ((a)*(a))

static float distanceSq(glm::vec3 a, glm::vec3 b)
{
    return SQ(a.x-b.x) + SQ(a.y-b.y) + SQ(a.z-b.z);
}


static glm::vec3 toglm( btVector3 v )
{
    return glm::vec3(v.getX(), v.getY(), v.getZ());
}

static btVector3 tobt(glm::vec3 v)
{
    return btVector3(v.x, v.y, v.z);
}



void
Grabbable_CS::stage_B( idk::Engine &engine )
{
    Physics_CS &pCS = engine.getCS<Physics_CS>("physics");
    Transform_CS &tCS = engine.getCS<Transform_CS>("transform");

    idk::Camera &cam = engine.rengine().getCamera();

    idk::Keylog &keylog = engine.keylog();
    const float dtime = engine.deltaTime();

    for (int obj_id: engine.gameObjects_byComponent(_component_index))
    {
        if (pCS.hasRigidBody(obj_id) == false)
            continue;

        btRigidBody *objectRB = pCS.getBtRigidBody(obj_id);

        idk::Transform &cam_transform = cam.transform();
        glm::vec3 grab_pos = cam_transform.position();
        glm::vec3 grab_point = grab_pos + GRAB_DIST*cam.front();

        glm::vec3 object_pos = toglm(objectRB->getWorldTransform().getOrigin());

        float distSq = distanceSq(grab_point, object_pos);

        if (distSq < 2.0f)
        {
            if (engine.mouseDown(idk::MouseButton::LEFT))
            {
                grabbed_id = obj_id;
            }
        }

        if (grabbed_id == -1)
            continue;

        btRigidBody *grabbedRB = pCS.getBtRigidBody(grabbed_id);

        glm::vec3 grabbed_pos = toglm(grabbedRB->getWorldTransform().getOrigin());
        distSq = distanceSq(grab_point, grabbed_pos);

        glm::vec3 dir = glm::normalize(grab_point - grabbed_pos);
        btVector3 btdir = tobt(distSq * dir);
        
        grabbedRB->activate(true);
        grabbedRB->applyCentralForce(btdir);
        grabbedRB->setLinearVelocity(btdir);
        grabbedRB->setAngularVelocity(btdir);
    

        if (keylog.keyDown(idk_keycode::Q))
        {
            grabbed_id = -1;
        }

        else if (keylog.keyDown(idk_keycode::E))
        {
            grabbedRB->setMassProps(10000.0f, btVector3(0, 0, 0));
            grabbedRB->applyCentralForce(tobt(50.0f*cam.front()));
            grabbedRB->setLinearVelocity(tobt(50.0f*cam.front()));
            grabbed_id = -1;
        }

    }
};
