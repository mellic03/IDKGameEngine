#include "IDK_grabbable.h"


void
Grabbable::init( idk::Engine &engine )
{
    _stage_B_predicate = [&engine](int idx)
    {
        auto &objt = engine.rengine().getTransform(engine.getGameObject(idx).transform_id);
        auto &pyrt = engine.rengine().getActiveCamera().transform();

        glm::vec3 player_pos = pyrt.position();
        glm::vec3 grabbable_pos = objt.position();

        float dist = glm::distance(player_pos, grabbable_pos);

        if ( 4.5 < dist && dist < 5.5f)
        {
            glm::vec3 dir = glm::normalize(grabbable_pos - player_pos);
            glm::vec3 front = engine.rengine().getActiveCamera().front();

            if (glm::dot(dir, front) > 0.9f && engine.mouseDown(idk::MouseButton::LEFT))
            {
                objt.position() = pyrt.position() + 5.0f * front;
            }
        }
    };
}



void
Grabbable::stage_B( idk::Engine &engine )
{
    engine.gameObjects_byComponent(_component_index)
    .for_each( _stage_B_predicate );
};


