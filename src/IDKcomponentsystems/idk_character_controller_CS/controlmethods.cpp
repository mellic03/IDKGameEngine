#include "controlmethods.h"
#include "../idk_physics_CS/idk_physics_CS.h"
#include "../idk_transform_CS.h"


static constexpr float SWAY_SPEED_LOOK   = 0.002f;
static constexpr float SWAY_SPEED_MOVE   = 0.01f;
static constexpr float SWAY_DECAY_FACTOR = 0.001f;
static float roll_sway  = 0.0f;
static float pitch_sway = 0.0f;

static void keyinput(idk::Camera &camera, idk::Keylog &keylog, float speed, float dtime, btRigidBody *rigidbody)
{

    auto removeY  = [](glm::vec3 v)
    {
        float length = glm::length(v); 
        v.y = 0.000000001f;
        return length * glm::normalize(v);
    };

    if (keylog.keyDown(idk_keycode::W))
    {
        glm::vec3 force = glm::vec3(0.0f, 0.0f, -speed) * glm::mat3(camera.view());
        b3::applyCentralForce(rigidbody, removeY(force));
    }

    if (keylog.keyDown(idk_keycode::S))
    {
        glm::vec3 force = glm::vec3(0.0f, 0.0f, +speed) * glm::mat3(camera.view());
        b3::applyCentralForce(rigidbody, removeY(force));
    }

    if (keylog.keyDown(idk_keycode::A))
    {
        glm::vec3 force = glm::vec3(-speed, 0.0f, 0.0f) * glm::mat3(camera.view());
        b3::applyCentralForce(rigidbody, removeY(force));
        roll_sway += speed * SWAY_SPEED_MOVE * dtime;
    }

    if (keylog.keyDown(idk_keycode::D))
    {
        glm::vec3 force = glm::vec3(+speed, 0.0f, 0.0f) * glm::mat3(camera.view());
        b3::applyCentralForce(rigidbody, removeY(force));
        roll_sway -= speed * SWAY_SPEED_MOVE * dtime;
    }
}

void
controlmethods::player( int obj_id, idk::Engine &engine )
{
    Transform_CS &tCS = engine.getCS<Transform_CS>("transform");
    Physics_CS &pCS = engine.getCS<Physics_CS>("physics");
    btRigidBody *rigidbody = pCS.getBtCapsule(obj_id);
    rigidbody->activate(true);
    rigidbody->setFriction(2.2f);
    btVector3 acc = btVector3(0.0, -10.0, 0.0);
    rigidbody->setGravity(acc);
    btVector3 vel = rigidbody->getLinearVelocity();
    rigidbody->setLinearVelocity({0.0, vel.getY(), 0.0});

    btVector3 pos = rigidbody->getWorldTransform().getOrigin();
    btVector3 btFrom(pos.getX(), pos.getY(), pos.getZ());
    btVector3 btTo(pos.getX(), -1000.0f, pos.getZ());
    btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

    pCS._physicsworld._dynamicsWorld->rayTest(btFrom, btTo, res);

    glm::vec3 slopeforce(0.0f);

    if(res.hasHit())
    {
        glm::vec3 hitpoint = glm::vec3(
            res.m_hitPointWorld.getX(),
            res.m_hitPointWorld.getY(),
            res.m_hitPointWorld.getZ()
        );

        glm::vec3 cam_pos = b3::glmvec3_cast(pos);
        glm::vec3 normal = b3::glmvec3_cast(res.m_hitNormalWorld);
        slopeforce = glm::vec3(0.0f, 1.0f, 0.0f) - normal;
    }
    rigidbody->applyCentralForce(b3::btVec3_cast(5.0f*slopeforce));


    idk::Camera &camera = engine.rengine().getCamera();
    idk::Transform &transform = tCS.getTransform(obj_id);
    idk::Keylog &keylog = engine.keylog();
    
    float dtime = engine.deltaTime();

    if (keylog.keyTapped(idk_keycode::C))
        engine.mouseCapture(!engine.mouseCaptured());

    float speed = 24.32f;

    if (keylog.keyDown(idk_keycode::LSHIFT))
        speed = 64.0f;


    camera.roll(-roll_sway);

    if (keylog.keyDown(idk_keycode::ESCAPE))
    {
        engine.shutdown();
    }

    keyinput(camera, keylog, speed, dtime, rigidbody);

    float decay = std::pow(SWAY_DECAY_FACTOR, dtime);
    roll_sway = idk::clamp(decay*roll_sway, -0.5f, 0.5f);

    if (keylog.keyDown(idk_keycode::SPACE))
    {
        // transform.translate(glm::vec3(0.0f,  speed*dtime*3.0f,  0.0f));
        rigidbody->applyCentralForce({0.0, +speed, 0.0});
    }

    if (keylog.keyDown(idk_keycode::LCTRL))
    {
        // transform.translate(glm::vec3(0.0f, -speed*dtime*3.0f,  0.0f));
        rigidbody->applyCentralForce({0.0, -speed, 0.0});
    }


    glm::vec3 rpos = b3::glmvec3_cast(pos);
    glm::vec3 campos = camera.transform().position();
    glm::vec3 dir = 10.0f * dtime * (rpos - campos);
    camera.transform().translate(dir);

    if (engine.mouseCaptured())
    {
        glm::vec2 dmouse = 0.001f * engine.dMouse();
        camera.transform().pitch(-dmouse.y);
        camera.transform().yaw(-dmouse.x);
        roll_sway -= speed * SWAY_SPEED_LOOK * dmouse.x;
    }

    camera.roll(roll_sway);
}

