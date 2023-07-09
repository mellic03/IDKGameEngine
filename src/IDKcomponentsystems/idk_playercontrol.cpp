#include "idk_playercontrol.h"
#include "idk_transform_CS.h"


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

    if (_current_player_obj == -1)
        return;

    idk::Camera &camera = engine.rengine().getCamera();
    idk::Transform &transform = tCS.getTransform(_current_player_obj);
    idk::Keylog &keylog = engine.keylog();
    
    float dtime = engine.deltaTime();

    if (keylog.keyTapped(idk_keycode::C))
        engine.mouseCapture(!engine.mouseCaptured());

    float speed = 2.84f;

    if (keylog.keyDown(idk_keycode::LSHIFT))
        speed = 8.0f;

    camera.roll(-roll_sway);
    // camera.pitch(-pitch_sway);

    if (engine.mouseCaptured())
    {
        glm::vec2 dmouse = 0.001f * engine.dMouse();
        transform.pitch(-dmouse.y);
        transform.yaw(-dmouse.x);
        roll_sway -= speed * SWAY_SPEED_LOOK * dmouse.x;
    }

    else if (keylog.keyDown(idk_keycode::ESCAPE))
    {
        engine.shutdown();
    }

    if (keylog.keyDown(idk_keycode::W))
    {
        transform.localTranslate(glm::vec3(0.0f, 0.0f, -speed*dtime));
    }

    if (keylog.keyDown(idk_keycode::S))
    {
        transform.localTranslate(glm::vec3(0.0f, 0.0f, speed*dtime));
    }

    if (keylog.keyDown(idk_keycode::A))
    {
        transform.localTranslate(glm::vec3(-speed*dtime, 0.0f, 0.0f));
        roll_sway += speed * SWAY_SPEED_MOVE * engine.deltaTime();
    }

    if (keylog.keyDown(idk_keycode::D))
    {
        transform.localTranslate(glm::vec3(speed*dtime, 0.0f, 0.0f));
        roll_sway -= speed * SWAY_SPEED_MOVE * engine.deltaTime();
    }

    float decay = std::pow(SWAY_DECAY_FACTOR, dtime);

    roll_sway = idk::clamp(decay*roll_sway, -0.5f, 0.5f);
    pitch_sway = idk::clamp(decay*pitch_sway, -0.2f, 0.2f);
    camera.roll(roll_sway);
    camera.pitch(pitch_sway);


    if (keylog.keyDown(idk_keycode::SPACE))
        transform.translate(glm::vec3(0.0f,  speed*dtime*3.0f,  0.0f));

    if (keylog.keyDown(idk_keycode::LCTRL))
        transform.translate(glm::vec3(0.0f, -speed*dtime*3.0f,  0.0f));
};


void
PlayerControl_CS::onAssignment( int obj_id, idk::Engine &engine )
{
    _current_player_obj = obj_id;
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


