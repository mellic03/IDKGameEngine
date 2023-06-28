#include "IDK_grabbable.h"
#include "IDK_physics.h"
#include "IDK_transform_CS.h"


constexpr float GRAB_DIST = 3.0f;
constexpr float GRAB_DIST_SQ = 9.0f;
constexpr float THROW_SPEED = 5000.0f;

static int grabbed_id = -1;


void
Grabbable_CS::stage_B( idk::Engine &engine )
{
    Physics_CS *pCS = engine.getCS<Physics_CS>("physics");
    Transform_CS *tCS = engine.getCS<Transform_CS>("transform");
    idk::Keylog &keylog = engine.keylog();
    const float dtime = engine.deltaTime();

    for (int obj_id: engine.gameObjects_byComponent(_component_index))
    {
        glm::vec3 &obj_pos = tCS->getTransform(obj_id).position();
        glm::vec3 cam_pos = engine.rengine().getActiveCamera().transform().position();
        glm::vec3 cam_front = engine.rengine().getActiveCamera().front();
        
        glm::vec3 grab_point = cam_pos + GRAB_DIST*cam_front;

        float distSQ = idk::distanceSq(grab_point, obj_pos);

        if (distSQ < GRAB_DIST_SQ)
        {
            if (keylog.keyDown(SDL_SCANCODE_E))
                grabbed_id = obj_id;
        }

        if (grabbed_id == -1)
        {
            continue;
        }


        glm::vec3 &grabbed_pos = tCS->getTransform(grabbed_id).position();        
        glm::vec3 &grabbed_vel = pCS->getVelocity(grabbed_id);        
        
        distSQ = idk::distanceSq(grab_point, grabbed_pos);
        glm::vec3 dir = distSQ * glm::normalize(grab_point - grabbed_pos);

        grabbed_vel += dir;
        grabbed_vel *= std::pow(0.001f, dtime);

        if (engine.mouseDown(idk::MouseButton::LEFT))
        {
            grabbed_vel = THROW_SPEED * dtime * cam_front;
            grabbed_id = -1;
        }

        else if (keylog.keyDown(SDL_SCANCODE_Q))
        {
            grabbed_vel = glm::vec3(0.0f);
            grabbed_id = -1;
        }
    }
};
