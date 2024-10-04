#include "sys-particle.hpp"
#include "sys-model.hpp"

void
idk::ParticleSys::init( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    float dt  = api.dtime();


    // for (auto &cmp: ECS2::getComponentArray<ParticleCmp>())
    // {
    //     if (cmp.src_id == -1)
    //     {
    //         continue;
    //     }

    //     glm::vec3 position = TransformSys::getPositionWorldspace(cmp.obj_id);
    //     int model = ECS2::getComponent<ModelCmp>(cmp.src_id).model_id;

    //     idk::ParticleEmitter P(cmp.desc, position);
    //     cmp.emitter_id = ren.createParticleEmitter(P, -1.0f);
    // }
}


void
idk::ParticleSys::update( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    float dt  = api.dtime();

    // for (auto &cmp: ECS2::getComponentArray<ParticleCmp>())
    // {
    //     if (cmp.src_id == -1)
    //     {
    //         continue;
    //     }

    //     glm::vec3 position = TransformSys::getPositionWorldspace(cmp.obj_id);
    //     int model = ECS2::getComponent<ModelCmp>(cmp.src_id).model_id;

    //     if (cmp.emitter_id == -1)
    //     {
    //         ParticleSystem::EmitterDesc ed = {
    //             .model_id  = model,
    //             .particles = 256,
    //             .duration  = -1.0f
    //         };

    //         ParticleSystem::ParticleDesc pd = {
    //             .origin_rng   = glm::vec3(0.0f),

    //             .velocity     = 2.0f * glm::vec3(0, 1, 0),
    //             .velocity_rng = 2.0f * glm::vec4(1.0f),

    //             .scale        = glm::vec3(0.25f),
    //             .scale_factor = glm::vec3(1.0f, 0.0f, 0.0f),
    //             .scale_rng    = 0.01f,

    //             .duration     = 1.5f,
    //             .duration_rng = glm::vec2(0.5f, 0.0f)
    //         };

    //         cmp.emitter_id = ParticleSystem::createEmitter(position, glm::vec3(0, 1, 0), ed, pd);
    //     }

    //     else
    //     {
    //         // auto &emitter = ParticleSystem::getEmitter(cmp.emitter_id);

    //         // emitter.position    = position;
    //         // emitter.velocity  = glm::vec3(0.0f); // TransformSys::getData(cmp.obj_id).delta;
    //         // emitter.direction = TransformSys::getFront(cmp.obj_id);
    //         // emitter.model_id  = model;
    //         // emitter.scale     = 1.0f; // TransformSys::getData(cmp.src_id).scale;
    //         // emitter.m_desc    = cmp.desc;
    //     }

    // }

}



size_t
idk::ParticleCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, src_id);
    n += idk::streamwrite(stream, desc);
    return n;
};


size_t
idk::ParticleCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, src_id);
    n += idk::streamread(stream, desc);

    emitter_id = -1;

    return n;
};


void
idk::ParticleCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    auto &cmp = idk::ECS2::getComponent<ParticleCmp>(obj_id);
    cmp.emitter_id = -1;


    // cmp.src_id = ECS2::createGameObject("source");

    // int model = api.getRenderer().loadModel("assets/models/fire-sprite.idkvi");
    // ECS2::giveComponent<ModelCmp>(cmp.src_id);
    // ECS2::getComponent<ModelCmp>(cmp.src_id).model_id = model;

    // cmp.desc = {
    //     .model_id            = model,
    //     .count               = 32,
    //     .velocity            = glm::vec3(0.0f, 0.0f, 15.0f),
    //     .velocity_randomness = glm::vec3(1.0f, 1.0f, 1.0f),
    //     .scale               = 1.0f,
    //     .scale_randomness    = 0.0f,
    //     .duration            = 1.0f,
    //     .duration_randomness = 0.0f
    // };

    // glm::vec3 position = TransformSys::getPositionWorldspace(cmp.obj_id);

    // idk::ParticleEmitter P(cmp.desc, position);
    // cmp.emitter_id = api.getRenderer().createParticleEmitter(P);

};


void
idk::ParticleCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    auto &cmp = idk::ECS2::getComponent<ParticleCmp>(obj_id);
    // api.getRenderer().destroyParticleEmitter(cmp.emitter_id);
};


void
idk::ParticleCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<ParticleCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<ParticleCmp>(dst_obj);
};


