#include "idk_playercontrol.h"

static constexpr float SWAY_SPEED_LOOK   = 0.01f;
static constexpr float SWAY_SPEED_MOVE   = 0.05f;
static constexpr float SWAY_DECAY_FACTOR = 0.001f;
static float roll_sway  = 0.0f;
static float pitch_sway = 0.0f;


void
PlayerControl_CS::stage_A(idk::Engine &engine)
{
    idk::Camera &camera = engine.rengine().getCamera();
    idk::Keylog &keylog = engine.keylog();
    
    float dtime = engine.deltaTime();

    if (keylog.keyTapped(idk_keycode::C))
        engine.mouseCapture(!engine.mouseCaptured());

    float speed = 2.84f;

    if (keylog.keyDown(idk_keycode::LSHIFT))
        speed = 8.0f;

    camera.roll(-roll_sway);
    camera.pitch(-pitch_sway);

    if (engine.mouseCaptured())
    {
        glm::vec2 dmouse = 0.001f * engine.dMouse();
        camera.pitch(dmouse.y);
        camera.yaw(dmouse.x);
        roll_sway -= speed * SWAY_SPEED_LOOK * dmouse.x;
    }

    else if (keylog.keyDown(idk_keycode::ESCAPE))
    {
        engine.shutdown();
    }

    if (keylog.keyDown(idk_keycode::W))
    {
        camera.translate(glm::vec3(0.0f, 0.0f, -speed*dtime));
    }

    if (keylog.keyDown(idk_keycode::S))
    {
        camera.translate(glm::vec3(0.0f, 0.0f, speed*dtime));
    }

    if (keylog.keyDown(idk_keycode::A))
    {
        camera.translate(glm::vec3(-speed*dtime, 0.0f, 0.0f));
        roll_sway += speed * SWAY_SPEED_MOVE * engine.deltaTime();
    }

    if (keylog.keyDown(idk_keycode::D))
    {
        camera.translate(glm::vec3(speed*dtime, 0.0f, 0.0f));
        roll_sway -= speed * SWAY_SPEED_MOVE * engine.deltaTime();
    }

    float decay = std::pow(SWAY_DECAY_FACTOR, dtime);

    roll_sway = idk::clamp(decay*roll_sway, -0.5f, 0.5f);
    pitch_sway = idk::clamp(decay*pitch_sway, -0.2f, 0.2f);
    camera.roll(roll_sway);
    camera.pitch(pitch_sway);


    if (keylog.keyDown(idk_keycode::SPACE))
        camera.elevation(speed * dtime * +3.0f);

    if (keylog.keyDown(idk_keycode::LCTRL))
        camera.elevation(speed * dtime * -3.0f);
};


