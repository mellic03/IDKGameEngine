#include "sys-audio.hpp"
#include <filesystem>


static idk::EngineAPI *api_ptr;

static idk::EngineAPI &getAPI()
{
    return *api_ptr;
};


void
idk::AudioSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;
    

    for (auto &[obj_id, emitter_id, volume, filepath]: ECS2::getComponentArray<AudioEmitterCmp>())
    {
        glm::vec3 pos = idk::TransformSys::getPositionWorldspace(obj_id);

        if (obj_id == -1 || emitter_id == -1)
        {
            continue;
        }

        int sound = loadSound(filepath);
        emitter_id = AudioSystem::createEmitter(idk::AudioSystem::Emitter(sound));
        AudioSystem::getEmitter(emitter_id).id = emitter_id;
    }

}


void
idk::AudioSys::update( idk::EngineAPI &api )
{
    static std::set<int> finished_callbacks;
    finished_callbacks.clear();

    for (auto &[obj_id, callback]: m_callbacks)
    {
        if (isFinished(obj_id))
        {
            callback();
            finished_callbacks.insert(obj_id);
        }
    }

    for (int obj_id: finished_callbacks)
    {
        m_callbacks.erase(obj_id);
    }


    for (auto &[obj_id]: ECS2::getComponentArray<AudioListenerCmp>())
    {
        glm::vec3 pos = idk::TransformSys::getPositionWorldspace(obj_id);
        glm::vec3 dir = idk::TransformSys::getFront(obj_id);

        AudioSystem::update(pos, dir);
    }


    for (auto &[obj_id, emitter_id, volume, filepath]: ECS2::getComponentArray<AudioEmitterCmp>())
    {
        glm::vec3 pos = idk::TransformSys::getPositionWorldspace(obj_id);

        if (obj_id == -1 || emitter_id == -1)
        {
            continue;
        }

        AudioSystem::getEmitter(emitter_id).pos = pos;
    }

}



int
idk::AudioSys::loadSound( const std::string &filepath )
{
    return AudioSystem::loadWav(filepath);
}


void
idk::AudioSys::assignSound( int obj_id, int sound )
{
    auto &cmp = ECS2::getComponent<AudioEmitterCmp>(obj_id);
    cmp.emitter_id = AudioSystem::createEmitter(idk::AudioSystem::Emitter(sound));

}

void
idk::AudioSys::assignSound( int obj_id, const std::string &filepath )
{
    auto &cmp = ECS2::getComponent<AudioEmitterCmp>(obj_id);

    int sound = loadSound(filepath);
    cmp.emitter_id = AudioSystem::createEmitter(idk::AudioSystem::Emitter(sound));
    cmp.filepath = filepath;
}


void
idk::AudioSys::playSoundCallback( int obj_id, std::function<void()> callback )
{
    auto &cmp = ECS2::getComponent<AudioEmitterCmp>(obj_id);
    m_callbacks[obj_id] = callback;
    AudioSystem::playSound(cmp.emitter_id, false);
}


void
idk::AudioSys::playSound( int obj_id, bool loop )
{
    auto &cmp = ECS2::getComponent<AudioEmitterCmp>(obj_id);
    AudioSystem::playSound(cmp.emitter_id, loop);
}


void
idk::AudioSys::stopSound( int obj_id )
{
    auto &cmp = ECS2::getComponent<AudioEmitterCmp>(obj_id);
    AudioSystem::stopSound(cmp.emitter_id);
}


void
idk::AudioSys::resumeSound( int obj_id )
{
    auto &cmp = ECS2::getComponent<AudioEmitterCmp>(obj_id);
    AudioSystem::resumeSound(cmp.emitter_id);
}


void
idk::AudioSys::pauseSound( int obj_id )
{
    auto &cmp = ECS2::getComponent<AudioEmitterCmp>(obj_id);
    AudioSystem::pauseSound(cmp.emitter_id);
}


bool
idk::AudioSys::isFinished( int obj_id )
{
    auto &cmp = ECS2::getComponent<AudioEmitterCmp>(obj_id);
    auto &emm = AudioSystem::getEmitter(cmp.emitter_id);

    return emm.channel == -1;
}


bool
idk::AudioSys::isPlaying( int obj_id )
{
    auto &cmp = ECS2::getComponent<AudioEmitterCmp>(obj_id);
    return AudioSystem::getEmitter(cmp.emitter_id).channel != -1;
}
