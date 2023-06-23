#include "IDK_playercontrol.h"


static constexpr float SWAY_SPEED_LOOK = 0.01f;
static constexpr float SWAY_SPEED_MOVE = 0.05f;


void
Builtin_PlayerControl::stage_A(idk::Engine &engine)
{
    idk::Camera &camera = engine.rengine().getActiveCamera();
    idk::Keylog &keylog = engine.keylog();
    
    float dtime = engine.deltaTime();

    if (keylog.keyTapped(SDL_SCANCODE_C))
        engine.mouseCapture(!engine.mouseCaptured());

    float speed = 5.0f;

    if (keylog.keyDown(SDL_SCANCODE_LSHIFT))
        speed = 8.0f;

    static float roll_sway = 0.0f;
    static float pitch_sway = 0.0f;
    camera.roll(-roll_sway);
    camera.pitch(-pitch_sway);

    
    if (engine.mouseCaptured())
    {
        glm::vec2 dmouse = 0.001f * engine.dMouse();
        camera.pitch(dmouse.y);
        camera.yaw(dmouse.x);
        roll_sway -= speed * SWAY_SPEED_LOOK * dmouse.x;
    }

    else if (keylog.keyDown(SDL_SCANCODE_ESCAPE))
    {
        engine.shutdown();
    }

    if (keylog.keyDown(SDL_SCANCODE_W))
    {
        camera.translate(glm::vec3(0.0f, 0.0f, -speed*dtime));
    }

    if (keylog.keyDown(SDL_SCANCODE_S))
    {
        camera.translate(glm::vec3(0.0f, 0.0f, speed*dtime));
    }

    if (keylog.keyDown(SDL_SCANCODE_A))
    {
        camera.translate(glm::vec3(-speed*dtime, 0.0f, 0.0f));
        roll_sway += speed * SWAY_SPEED_MOVE * engine.deltaTime();
    }

    if (keylog.keyDown(SDL_SCANCODE_D))
    {
        camera.translate(glm::vec3(speed*dtime, 0.0f, 0.0f));
        roll_sway -= speed * SWAY_SPEED_MOVE * engine.deltaTime();
    }

    float decay = std::pow(0.001f, dtime);

    roll_sway = idk::clamp(decay*roll_sway, -0.5f, 0.5f);
    pitch_sway = idk::clamp(decay*pitch_sway, -0.2f, 0.2f);
    camera.roll(roll_sway);
    camera.pitch(pitch_sway);


    if (keylog.keyDown(SDL_SCANCODE_SPACE))
        camera.elevation(speed * dtime * +3.0f);

    if (keylog.keyDown(SDL_SCANCODE_LCTRL))
        camera.elevation(speed * dtime * -3.0f);
};


