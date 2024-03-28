#include "sys-audio.hpp"
#include "idk_systems.hpp"
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
    auto &ecs   = api.getECS();
    auto &audio = api.getAudioSys();

    for (auto &[obj_id, emitter_id, volume, filepath]: ecs.getComponentArray<AudioEmitterCmp>())
    {
        glm::vec3 pos = idk::TransformSys::getPositionWorldspace(obj_id);

        if (obj_id == -1 || emitter_id == -1)
        {
            continue;
        }

        std::cout << filepath << "\n";

        std::cout << "A\n";
        int sound = loadSound(filepath);
        std::cout << "B\n";
        emitter_id = audio.createEmitter(idk::AudioSystem::Emitter(sound));
        std::cout << "C\n";
        audio.getEmitter(emitter_id).id = emitter_id;
        std::cout << "D\n";
    }

}


void
idk::AudioSys::update( idk::EngineAPI &api )
{
    auto &ecs   = getAPI().getECS();
    auto &audio = getAPI().getAudioSys();

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


    for (auto &[obj_id]: ecs.getComponentArray<AudioListenerCmp>())
    {
        glm::vec3 pos = idk::TransformSys::getPositionWorldspace(obj_id);
        glm::vec3 dir = idk::TransformSys::getFront(obj_id);

        audio.update(pos, dir);
    }


    for (auto &[obj_id, emitter_id, volume, filepath]: ecs.getComponentArray<AudioEmitterCmp>())
    {
        glm::vec3 pos = idk::TransformSys::getPositionWorldspace(obj_id);

        if (obj_id == -1 || emitter_id == -1)
        {
            continue;
        }

        audio.getEmitter(emitter_id).pos = pos;
    }

}


void
idk::AudioSys::exposeToLua( lua_State *LS )
{
    luaaa::LuaModule mod(LS, "AudioSys");
    mod.fun("playSound",   idk::AudioSys::playSound);
    mod.fun("stopSound",   idk::AudioSys::stopSound);
    mod.fun("resumeSound", idk::AudioSys::resumeSound);
    mod.fun("pauseSound",  idk::AudioSys::pauseSound);
    mod.fun("isPlaying",   idk::AudioSys::isPlaying);
}



int
idk::AudioSys::loadSound( const std::string &filepath )
{
    return getAPI().getAudioSys().loadWav(filepath);
}


void
idk::AudioSys::assignSound( int obj_id, int sound )
{
    auto &api   = getAPI();
    auto &ecs   = api.getECS();
    auto &audio = api.getAudioSys();

    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    cmp.emitter_id = audio.createEmitter(idk::AudioSystem::Emitter(sound));
    audio.getEmitter(cmp.emitter_id).id = cmp.emitter_id;

}

void
idk::AudioSys::assignSound( int obj_id, const std::string &filepath )
{
    auto &api   = getAPI();
    auto &ecs   = api.getECS();
    auto &audio = api.getAudioSys();

    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);

    int sound = loadSound(filepath);
    cmp.emitter_id = audio.createEmitter(idk::AudioSystem::Emitter(sound));
    audio.getEmitter(cmp.emitter_id).id = cmp.emitter_id;
    cmp.filepath = filepath;
}


void
idk::AudioSys::playSoundCallback( int obj_id, std::function<void()> callback )
{
    auto &api   = getAPI();
    auto &ecs   = api.getECS();
    auto &audio = api.getAudioSys();

    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);

    m_callbacks[obj_id] = callback;
    audio.playSound(cmp.emitter_id, false);
}


void
idk::AudioSys::playSound( int obj_id, bool loop )
{
    auto &api   = getAPI();
    auto &ecs   = api.getECS();
    auto &audio = api.getAudioSys();

    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    audio.playSound(cmp.emitter_id, loop);
}


void
idk::AudioSys::stopSound( int obj_id )
{
    auto &api   = getAPI();
    auto &ecs   = api.getECS();
    auto &audio = api.getAudioSys();

    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    audio.stopSound(cmp.emitter_id);
}


void
idk::AudioSys::resumeSound( int obj_id )
{
    auto &api   = getAPI();
    auto &ecs   = api.getECS();
    auto &audio = api.getAudioSys();

    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    audio.resumeSound(cmp.emitter_id);
}


void
idk::AudioSys::pauseSound( int obj_id )
{
    auto &api   = getAPI();
    auto &ecs   = api.getECS();
    auto &audio = api.getAudioSys();

    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    audio.pauseSound(cmp.emitter_id);
}


bool
idk::AudioSys::isFinished( int obj_id )
{
    auto &api   = getAPI();
    auto &ecs   = api.getECS();
    auto &audio = api.getAudioSys();

    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    auto &emm = audio.getEmitter(cmp.emitter_id);

    return emm.channel == -1;
}


bool
idk::AudioSys::isPlaying( int obj_id )
{
    auto &api   = getAPI();
    auto &ecs   = api.getECS();
    auto &audio = api.getAudioSys();

    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    return audio.getEmitter(cmp.emitter_id).channel != -1;
}
