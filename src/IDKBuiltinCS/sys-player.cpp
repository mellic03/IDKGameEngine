#include "sys-player.hpp"

#include <IDKEvents/IDKEvents.hpp>
#include "sys-transform.hpp"
#include "sys-physx.hpp"
#include "sys-planet.hpp"



void
idk::PlayerSys::init( idk::EngineAPI &api )
{

}



static int PLAYER_ID = -1;

void
idk::PlayerSys::update( idk::EngineAPI &api )
{
    if (PLAYER_ID == -1)
    {
        return;
    }

    static physx::PxController *controller = idk::PhysXSys::createCapsuleKinematic(PLAYER_ID, 1.75f, 0.5f);


    auto  &engine = api.getEngine();
    auto  &events = api.getEventSys();
    auto  &keylog = events.keylog();
    auto  &ecs    = api.getECS();


    glm::vec3 G = glm::vec3(0.0f);

    G = PlanetSys::averageGravity(TransformSys::getPositionWorldspace(PLAYER_ID));

    if (G == glm::vec3(0.0f))
    {
        G = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    TransformSys::setSurfaceUp(PLAYER_ID, glm::normalize(-G), 8.0f*engine.deltaTime());


    if (events.mouseCaptured())
    {
        _keyInput(api, PLAYER_ID);
        _mouseInput(api, PLAYER_ID);
    }

}




void
idk::PlayerSys::_keyInput( idk::EngineAPI &api, int player )
{
    auto  &engine = api.getEngine();
    auto  &events = api.getEventSys();
    auto  &keylog = events.keylog();
    auto  &ecs    = api.getECS();
    float dtime   = engine.deltaTime();

    glm::vec3 delta = glm::vec3(0.0f);

    glm::vec3 front = TransformSys::getSurfaceFront(player);
    glm::vec3 right = TransformSys::getSurfaceRight(player);
    glm::vec3 up    = TransformSys::getSurfaceUp(player);


    float speed = 4.0f * dtime;

    if (keylog.keyDown(idk::Keycode::W))
    {
        delta += speed * front;
    }

    if (keylog.keyDown(idk::Keycode::S))
    {
        delta -= speed * front;
    }

    if (keylog.keyDown(idk::Keycode::A))
    {
        delta -= speed * right;
    }

    if (keylog.keyDown(idk::Keycode::D))
    {
        delta += speed * right;
    }


    if (keylog.keyDown(idk::Keycode::SPACE))
    {
        delta += speed * up;
    }

    if (keylog.keyDown(idk::Keycode::LCTRL))
    {
        delta -= speed * up;
    }


    TransformSys::translateWorldspace(player, delta);

}


void
idk::PlayerSys::_mouseInput( idk::EngineAPI &api, int player )
{
    auto  &engine = api.getEngine();
    auto  &events = api.getEventSys();
    auto  &keylog = events.keylog();
    auto  &ecs    = api.getECS();
    float dtime   = engine.deltaTime();


    glm::vec2 dmouse = -0.035f * events.mouseDelta();

    TransformSys::yaw(player, dmouse.x);
    TransformSys::pitch(player, dmouse.y);

}





size_t
idk::PlayerControllerCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
}


size_t
idk::PlayerControllerCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
}


void
idk::PlayerControllerCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    this->obj_id = obj_id;
    PLAYER_ID = obj_id;
}


void
idk::PlayerControllerCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

}


void
idk::PlayerControllerCmp::onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
{
    IDK_ASSERT("Cannot copy idk::PlayerControllerCmp!", false);
}



