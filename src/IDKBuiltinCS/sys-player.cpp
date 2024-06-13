#include "sys-player.hpp"

#include "sys-transform.hpp"
#include "sys-physics.hpp"

#include <IDKEvents/IDKEvents.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKThreading/IDKThreading.hpp>
#include <libidk/idk_log.hpp>



void
idk::PlayerSys::init( idk::EngineAPI &api )
{

}




void
idk::PlayerSys::update( idk::EngineAPI &api )
{
    auto &ecs = api.getECS();

    for (auto &cmp: ecs.getComponentArray<idk::PlayerControllerCmp>())
    {
        cmp.input(api);
        cmp.update(api);
    }

    for (auto &cmp: ecs.getComponentArray<idk::ArmCmp>())
    {
        cmp.update(api);
        cmp.draw(api);
    }
}



void
idk::PlayerControllerCmp::update( idk::EngineAPI &api )
{
    float dtime  = api.getEngine().deltaTime();
    auto &ren    = api.getRenderer();
    auto &ecs    = api.getECS();
    auto &events = api.getEventSys();
    auto &K      = events.keylog();

    if (model_obj == -1)
    {
        model_obj = ecs.createGameObject("Model");
        ecs.giveComponent<TransformCmp>(model_obj);

        ecs.giveChild(obj_id, model_obj);
        TransformSys::setPositionLocalspace(model_obj, glm::vec3(0.0f));
    }

    TransformSys::getData(obj_id).yaw = TransformSys::getData(hinge_obj).yaw;


    static float G = 9.8f;
    static glm::vec3 acc = glm::vec3(0.0f, 0.0f, 0.0f);
    static glm::vec3 vel = glm::vec3(0.0f, 0.0f, 0.0f);

    auto &cmp = ecs.getComponent<idk::KinematicCapsuleCmp>(obj_id);

    acc.y = -PhysicsConstants::G;

    if (cmp.enabled)
    {
        if (cmp.grounded)
        {
            vel.y *= 0.5f;

            if (K.keyDown(idk::Keycode::SPACE))
            {
                vel.y = jump_force*PhysicsConstants::G;
                // glm::vec3 f = glm::vec3(0.0f, jump_force*PhysicsConstants::G, 0.0f);
                // PhysicsSys::addForce(obj_id, f);
            }

            if (K.keyDown(idk::Keycode::LCTRL))
            {
                cmp.bottom = 0.5f;
            }

            else if (cmp.bottom < 0.75f)
            {
                cmp.bottom += 2.0f*dtime;
            }
        }

        vel = glm::clamp(vel, glm::vec3(-100.0f), glm::vec3(+100.0f));
        vel += dtime*acc;
        glm::vec3 dP = vel + 0.5f*dtime*acc;
        PhysicsSys::addForce(obj_id, dP);
    }

    idkui::TextManager::text(10, 150) << "acc: " << acc.y;
    idkui::TextManager::text(10, 175) << "vel: " << vel.y;
}



void
idk::PlayerControllerCmp::input( idk::EngineAPI &api )
{
    float dtime  = api.getEngine().deltaTime();
    auto &ren    = api.getRenderer();
    auto &ecs    = api.getECS();
    auto &events = api.getEventSys();
    auto &K      = events.keylog();

    glm::vec3 delta = glm::vec3(0.0f);
    glm::vec3 up    = TransformSys::getUp(obj_id);
    glm::vec3 right = TransformSys::getRight(obj_id);
    glm::vec3 front = TransformSys::getFront(obj_id);


    if (K.keyDown(idk::Keycode::A)) delta -= right;
    if (K.keyDown(idk::Keycode::D)) delta += right;
    if (K.keyDown(idk::Keycode::W)) delta += front;
    if (K.keyDown(idk::Keycode::S)) delta -= front;

    delta.y = 0.0f;

    if (fabs(delta.x) > 0.01f || fabs(delta.z) > 0.01f)
    {
        delta = walk_speed * glm::normalize(delta);
    }

    PhysicsSys::addForce(obj_id, delta);

    // idk::TransformSys::translateWorldspace(obj_id, delta);


    if (fabs(delta.x) > 0.01f || fabs(delta.z) > 0.01f)
    {
        delta = dtime * walk_speed * glm::normalize(delta);
    }

    if (K.keyTapped(idk::Keycode::ESCAPE))
    {
        bool captured = events.mouseCaptured();
        events.mouseCapture(!captured);
    }

    if (events.mouseCaptured())
    {
        glm::vec2 dmouse = -events.mouseDelta();
        float mouse_speed = 0.1f;

        TransformSys::yaw(hinge_obj, mouse_speed*dmouse.x);
        TransformSys::pitch(hinge_obj, mouse_speed*dmouse.y);
    }
}



void
idk::ArmCmp::update( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    auto &ecs = api.getECS();
    float dt  = api.getEngine().deltaTime();

    int parent_id = ecs.getParent(obj_id);

    if (partner_id == -1 || rest_target == -1 || ray_target == -1 || pole_target == -1)
    {
        return;
    }

    auto &partner = ecs.getComponent<ArmCmp>(partner_id);

    glm::vec3 parent_pos = TransformSys::getPositionWorldspace(parent_id);
    glm::vec3 avg = 0.5f * (hand_pos + partner.hand_pos);

    parent_pos.y = 0.0f;
    avg.y = 0.0f;

    if (moving == partner.moving)
    {
        moving = false;
        partner.moving = false;
    }

    // if (glm::distance(parent_pos, avg) > stride/2.0f)
    // {
    //     glm::vec3 a = glm::vec3(1.0f, 0.0, 1.0f) * hand_pos;
    //     glm::vec3 b = glm::vec3(1.0f, 0.0, 1.0f) * partner.hand_pos;

    //     if (glm::distance(parent_pos, a) < glm::distance(parent_pos, b))
    //     {
    //         moving = true;
    //     }

    //     else
    //     {
    //         // moving = false;
    //     }
    // }


    glm::vec3 shoulder_pos = TransformSys::getPositionWorldspace(obj_id);
    glm::vec3 target = TransformSys::getPositionWorldspace(pole_target);

    glm::vec3 rest_pos   = TransformSys::getPositionWorldspace(rest_target);
    glm::vec3 ray_origin = TransformSys::getPositionWorldspace(ray_target);
    glm::vec3 ray_dir    = glm::vec3(0.0f, -1.0f, 0.0000001f);

    glm::vec3 hit;

    glm::vec3 delta = TransformSys::getData(parent_id).delta;

    if (PhysicsSys::raycast(ray_origin, ray_dir, hit))
    {
        float dist1 = glm::distance(ray_origin, hit);
        float dist2 = glm::distance(hand_pos, hit);

        if (dist2 >= stride)
        {
            next_anchor_pos = hit;

            m_alpha = 0.0f;
        }
    }


    ren.drawSphere(next_anchor_pos, 0.1f);

    if (PhysicsSys::raycast(hand_pos, ray_dir, hit))
    {
        float dist = glm::distance(hand_pos, hit);
        grounded = (dist < 0.001f);
    }


    float height = step_height * sin(m_alpha*M_PI);

    // if (!grounded && !partner.grounded)
    // {
    //     // anchor_pos = hit;
    //     m_alpha = 0.0f;
    //     anchor_pos = rest_pos;
    //     target_pos = rest_pos;
    // }

    if (m_alpha < 1.0f)
    {
        if (partner.grounded)
        {
            anchor_pos = glm::mix(prev_anchor_pos, next_anchor_pos, m_alpha);
            anchor_pos.y += height;
            m_alpha += step_speed * dt;
        }
    }

    else
    {
        prev_anchor_pos = next_anchor_pos;
    }

    // anchor_pos.x += shoulder_pos.x - anchor_pos.x;

    target_pos += 128.0f * dt * (anchor_pos - target_pos);

    TransformSys::FABRIK(
        shoulder_pos, elbow_pos, hand_pos, target_pos, target, distAB, distBC
    );

}



void
idk::ArmCmp::state_arrived( idk::EngineAPI &api )
{


}


void
idk::ArmCmp::state_moving( idk::EngineAPI &api )
{

}







void
idk::ArmCmp::draw( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();

    glm::vec3 shoulder_pos = TransformSys::getPositionWorldspace(obj_id);

    ren.drawCapsule(shoulder_pos, elbow_pos, 0.025f);
    ren.drawCapsule(elbow_pos, hand_pos, 0.025f);
}





size_t
idk::PlayerControllerCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, hinge_obj);
    n += idk::streamwrite(stream, cam_obj);
    n += idk::streamwrite(stream, model_obj);
    n += idk::streamwrite(stream, walk_speed);
    n += idk::streamwrite(stream, run_speed);
    n += idk::streamwrite(stream, jump_force);
    return n;
}


size_t
idk::PlayerControllerCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, hinge_obj);
    n += idk::streamread(stream, cam_obj);
    n += idk::streamread(stream, model_obj);
    n += idk::streamread(stream, walk_speed);
    n += idk::streamread(stream, run_speed);
    n += idk::streamread(stream, jump_force);
    return n;
}


void
idk::PlayerControllerCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    LOG_INFO() << "idk::PlayerControllerCmp::onObjectAssignment";

    this->obj_id = obj_id;

}


void
idk::PlayerControllerCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    LOG_INFO() << "idk::PlayerControllerCmp::onObjectDeassignment";
}


void
idk::PlayerControllerCmp::onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
{
    IDK_ASSERT("Cannot copy idk::PlayerControllerCmp!", false);
}








size_t
idk::ArmCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, partner_id);
    n += idk::streamwrite(stream, is_left);
    n += idk::streamwrite(stream, rest_target);
    n += idk::streamwrite(stream, ray_target);
    n += idk::streamwrite(stream, pole_target);
    n += idk::streamwrite(stream, distAB);
    n += idk::streamwrite(stream, distBC);
    n += idk::streamwrite(stream, stride);
    n += idk::streamwrite(stream, step_speed);
    n += idk::streamwrite(stream, step_height);
    n += idk::streamwrite(stream, elbow_pos);
    n += idk::streamwrite(stream, hand_pos);
    n += idk::streamwrite(stream, target_pos);
    return n;
}


size_t
idk::ArmCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, partner_id);
    n += idk::streamread(stream, is_left);
    n += idk::streamread(stream, rest_target);
    n += idk::streamread(stream, ray_target);
    n += idk::streamread(stream, pole_target);
    n += idk::streamread(stream, distAB);
    n += idk::streamread(stream, distBC);
    n += idk::streamread(stream, stride);
    n += idk::streamread(stream, step_speed);
    n += idk::streamread(stream, step_height);
    n += idk::streamread(stream, elbow_pos);
    n += idk::streamread(stream, hand_pos);
    n += idk::streamread(stream, target_pos);
    return n;
}


void
idk::ArmCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    LOG_INFO() << "idk::ArmCmp::onObjectAssignment";

    this->obj_id = obj_id;

}


void
idk::ArmCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    LOG_INFO() << "idk::ArmCmp::onObjectDeassignment";
}


void
idk::ArmCmp::onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
{

}



