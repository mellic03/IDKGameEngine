#include "idk_playercontrol.h"
#include "idk_transform_CS.h"
#include "idk_physics_CS/idk_physics_CS.h"


static constexpr float SWAY_SPEED_LOOK   = 0.01f;
static constexpr float SWAY_SPEED_MOVE   = 0.05f;
static constexpr float SWAY_DECAY_FACTOR = 0.001f;
static float roll_sway  = 0.0f;
static float pitch_sway = 0.0f;


void
PlayerControl_CS::init(idk::Engine &engine)
{
    addDependencies("transform", "physics", "camera");
}



void
PlayerControl_CS::stage_A(idk::Engine &engine)
{
    Transform_CS &tCS = engine.getCS<Transform_CS>("transform");
    Physics_CS &pCS = engine.getCS<Physics_CS>("physics");
    btRigidBody *rigidbody = pCS.getBtCapsule(_current_player_obj);
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

    pCS._physicsworld._dynamicsWorld->rayTest(btFrom, btTo, res); // m_btWorld is btDiscreteDynamicsWorld

    glm::vec3 slopeforce(0.0f);

    if(res.hasHit())
    {
        glm::vec3 hitpoint = glm::vec3(
            res.m_hitPointWorld.getX(),
            res.m_hitPointWorld.getY(),
            res.m_hitPointWorld.getZ()
        );

        glm::vec3 cam_pos = bullet3_tools::glmvec3_cast(pos);
        glm::vec3 normal = bullet3_tools::glmvec3_cast(res.m_hitNormalWorld);
        slopeforce = glm::vec3(0.0f, 1.0f, 0.0f) - normal;
    }
    rigidbody->applyCentralForce(bullet3_tools::btVec3_cast(5.0f*slopeforce));


    if (_current_player_obj == -1)
        return;

    idk::Camera &camera = engine.rengine().getCamera();
    idk::Transform &transform = tCS.getTransform(_current_player_obj);
    idk::Keylog &keylog = engine.keylog();
    
    float dtime = engine.deltaTime();

    if (keylog.keyTapped(idk_keycode::C))
        engine.mouseCapture(!engine.mouseCaptured());

    float speed = 24.32f;

    if (keylog.keyDown(idk_keycode::LSHIFT))
        speed = 64.0f;


    // camera.roll(-roll_sway);
    // camera.pitch(-pitch_sway);

    if (keylog.keyDown(idk_keycode::ESCAPE))
    {
        engine.shutdown();
    }

    if (keylog.keyDown(idk_keycode::W))
    {
        glm::vec3 force = glm::vec3(0.0f, 0.0f, -speed);
        force = force * glm::mat3(camera.view());
        force.y = 0.00001f;
        force = speed * glm::normalize(force);
        rigidbody->applyCentralForce(bullet3_tools::btVec3_cast(force));
    }

    if (keylog.keyDown(idk_keycode::S))
    {
        glm::vec3 force = glm::vec3(0.0f, 0.0f, +speed);
        force = force * glm::mat3(camera.view());
        force.y = 0.00001f;
        force = speed * glm::normalize(force);
        rigidbody->applyCentralForce(bullet3_tools::btVec3_cast(force));
    }

    if (keylog.keyDown(idk_keycode::A))
    {
        glm::vec3 force = glm::vec3(-speed, 0.0f, 0.0f);
        force = force * glm::mat3(camera.view());
        rigidbody->applyCentralForce(bullet3_tools::btVec3_cast(force));
        roll_sway += speed * SWAY_SPEED_MOVE * engine.deltaTime();
    }

    if (keylog.keyDown(idk_keycode::D))
    {
        glm::vec3 force = glm::vec3(+speed, 0.0f, 0.0f);
        force = force * glm::mat3(camera.view());
        rigidbody->applyCentralForce(bullet3_tools::btVec3_cast(force));
        roll_sway -= speed * SWAY_SPEED_MOVE * engine.deltaTime();
    }

    float decay = std::pow(SWAY_DECAY_FACTOR, dtime);

    roll_sway = idk::clamp(decay*roll_sway, -0.5f, 0.5f);
    pitch_sway = idk::clamp(decay*pitch_sway, -0.2f, 0.2f);


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


    glm::vec3 rpos = bullet3_tools::glmvec3_cast(pos);
    glm::vec3 campos = camera.transform().position();
    glm::vec3 dir = 10.0f * dtime * (rpos - campos);
    camera.transform().translate(dir);
  

    if (engine.mouseCaptured())
    {
        glm::vec2 dmouse = 0.001f * engine.dMouse();
        camera.transform().pitch(-dmouse.y);
        camera.transform().yaw(-dmouse.x);
        // roll_sway -= speed * SWAY_SPEED_LOOK * dmouse.x;
    }

};


void
PlayerControl_CS::onAssignment( int obj_id, idk::Engine &engine )
{
    _current_player_obj = obj_id;
  
    Physics_CS &pCS = engine.getCS<Physics_CS>("physics");

}


void
PlayerControl_CS::onGameObjectCreation( int obj_id, idk::Engine &engine )
{

}


void
PlayerControl_CS::onGameObjectDeletion( int obj_id, idk::Engine &engine )
{
    if (obj_id == _current_player_obj)
        _current_player_obj = -1;
}


void
PlayerControl_CS::onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{

}


