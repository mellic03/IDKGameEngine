#include "sys-particle.hpp"
#include "sys-model.hpp"



void
idk::ParticleSys::update( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();

    for (auto &cmp: ECS2::getComponentArray<ParticleCmp>())
    {
        if (cmp.src_id == -1)
        {
            continue;
        }

        glm::vec3 position = TransformSys::getPositionWorldspace(cmp.obj_id);
        int model = ECS2::getComponent<ModelCmp>(cmp.src_id).model_id;

        if (cmp.emitter_id == -1)
        {
            idk::ParticleEmitter P(cmp.desc, position);
            cmp.emitter_id = ren.createParticleEmitter(P);
        }

        auto &emitter = ren.getParticleEmitter(cmp.emitter_id);

        emitter.origin    = position;
        emitter.velocity  = glm::vec3(0.0f); // TransformSys::getData(cmp.obj_id).delta;
        emitter.direction = TransformSys::getFront(cmp.obj_id);
        emitter.model_id  = model;
        emitter.scale     = 1.0f; // TransformSys::getData(cmp.src_id).scale;
        emitter.m_desc    = cmp.desc;

    }

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

    cmp.src_id = ECS2::createGameObject("source");

    int model = api.getRenderer().loadModel("assets/models/fire-sprite.idkvi");
    ECS2::giveComponent<ModelCmp>(cmp.src_id);
    ECS2::getComponent<ModelCmp>(cmp.src_id).model_id = model;

    cmp.desc = {
        .model_id            = model,
        .count               = 32,
        .velocity            = glm::vec3(0.0f, 0.0f, 15.0f),
        .velocity_randomness = glm::vec3(1.0f, 1.0f, 1.0f),
        .scale               = 1.0f,
        .scale_randomness    = 0.0f,
        .duration            = 1.0f,
        .duration_randomness = 0.0f
    };

    glm::vec3 position = TransformSys::getPositionWorldspace(cmp.obj_id);

    idk::ParticleEmitter P(cmp.desc, position);
    cmp.emitter_id = api.getRenderer().createParticleEmitter(P);

};


void
idk::ParticleCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    auto &cmp = idk::ECS2::getComponent<ParticleCmp>(obj_id);
    api.getRenderer().destroyParticleEmitter(cmp.emitter_id);
};


void
idk::ParticleCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<ParticleCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<ParticleCmp>(dst_obj);
};


