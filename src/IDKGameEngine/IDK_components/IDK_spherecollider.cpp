#include "IDK_spherecollider.h"
#include "IDK_physics.h"
#include "IDK_transform_CS.h"


#define SQ(a) ((a)*(a))
static float distanceSq(glm::vec3 a, glm::vec3 b)
{
    return SQ(a.x - b.x) + SQ(a.y - b.y) + SQ(a.z - b.z);
}

static float calculate_impulse(glm::vec3 &vel, glm::vec3 &normal, float mass1, float mass2, float elasticity)
{
    float relative_to_normal = glm::dot(vel, normal);
    if (relative_to_normal > 0)
        return 0.0f;
    
    float j = -(1 + elasticity) * relative_to_normal;
    if (mass2 == 0)
        mass2 = 10000000;
    j /= (1/mass1) + 0.00001;

    return j;
}


void
SphereCollider_CS::stage_A( idk::Engine &engine )
{
    Physics_CS *PhysicsComponent = engine.getCS<Physics_CS>("physics");
    Transform_CS *tcomp = engine.getCS<Transform_CS>("transform");

    idk::RenderEngine &ren = engine.rengine();

    std::vector<int> obj_ids = engine.gameObjects_byComponent(_component_index);

    for (int i=0; i<obj_ids.size(); i++)
    {
        int obj1 = obj_ids[i];

        // idk::Transform transform = tcomp->getTransform(obj1);
        // transform.scale(glm::vec3(_radii[obj1]));
        // ren.drawWireframe(ren.wireframe_shader, ren.SPHERE_PRIMITIVE, transform);
        
        for (int j=i+1; j<obj_ids.size(); j++)
        {
            int obj2 = obj_ids[j];

            float rad1 = _radii[obj1];
            float rad2 = _radii[obj2];

            idk::Transform &t1 = tcomp->getTransform(obj1);
            idk::Transform &t2 = tcomp->getTransform(obj2);

            float dist = glm::distance(t1.position(), t2.position());

            if (dist < rad1 + rad2)
            {
                glm::vec3 dir = glm::normalize(t1.position() - t2.position());
                
                float overlap = rad1 + rad2 - dist;
                dir = overlap * dir;

                PhysicsComponent->getVelocity(obj1) += dir;
                PhysicsComponent->getVelocity(obj2) -= dir;
            }
        }
    }
}

#undef SQ


void
SphereCollider_CS::onGameObjectCreation( int obj_id )
{
    if (obj_id >= _radii.size())
        _radii.resize(obj_id+1, 0.0f);
    else
        _radii[obj_id] = 0.0f;
}


void
SphereCollider_CS::onGameObjectDeletion( int obj_id )
{
    _radii[obj_id] = 0.0f;
}


void
SphereCollider_CS::onGameObjectCopy( int src_obj_id, int dest_obj_id )
{
    _radii[dest_obj_id] = _radii[src_obj_id];
}
