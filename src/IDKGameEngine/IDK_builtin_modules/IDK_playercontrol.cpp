#include "IDK_playercontrol.h"



void
idk::builtin_modules::Builtin_PlayerControl::stage_B(idk::Engine &engine)
{
    idk::Camera &camera = engine.rengine().getActiveCamera();
    idk::Keylog &keylog = engine.keylog();
    
    float dtime = engine.deltaTime();
    
    if (engine.mouseCaptured())
    {
        glm::vec2 dmouse = dtime * engine.dMouse();
        camera.yaw(dmouse.x);
        camera.pitch(dmouse.y);
    }

    else if (keylog.keyDown(SDL_SCANCODE_ESCAPE))
        engine.shutdown();

    if (keylog.keyTapped(SDL_SCANCODE_E))
        engine.mouseCapture(!engine.mouseCaptured());

    if (keylog.keyDown(SDL_SCANCODE_W))
        camera.translate(glm::vec3(0.0f, 0.0f, dtime * +3.01f));

    if (keylog.keyDown(SDL_SCANCODE_S))
        camera.translate(glm::vec3(0.0f, 0.0f, dtime * -3.01f));

    if (keylog.keyDown(SDL_SCANCODE_A))
        camera.translate(glm::vec3(dtime * +3.01f, 0.0f, 0.0f));

    if (keylog.keyDown(SDL_SCANCODE_D))
        camera.translate(glm::vec3(dtime * -3.01f, 0.0f, 0.0f));

    if (keylog.keyDown(SDL_SCANCODE_SPACE))
        camera.elevation(dtime * -3.01f);

    if (keylog.keyDown(SDL_SCANCODE_LCTRL))
        camera.elevation(dtime * +3.01f);

};


