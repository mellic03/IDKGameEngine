#include "sys-audio.hpp"
#include "idk_systems.hpp"


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

    for (auto &[obj_id, emitter_id, volume, filepath]: ecs.getComponentArray<AudioEmitterCmp>().data())
    {
        glm::vec3 pos = idk::TransformSys::getPosition(obj_id);

        if (obj_id == -1 || emitter_id == -1)
        {
            continue;
        }

        int sound  = loadSound(filepath);
        emitter_id = audio.createEmitter(idk::AudioSystem::Emitter(sound));
    }

}


void
idk::AudioSys::update( idk::EngineAPI &api )
{
    auto &ecs   = getAPI().getECS();
    auto &audio = getAPI().getAudioSys();

    for (auto &[obj_id]: ecs.getComponentArray<AudioListenerCmp>().data())
    {
        glm::vec3 pos = idk::TransformSys::getPosition(obj_id);
        glm::vec3 dir = idk::TransformSys::getFront(obj_id);

        audio.update(pos, dir);
    }


    for (auto &[obj_id, emitter_id, volume, filepath]: ecs.getComponentArray<AudioEmitterCmp>().data())
    {
        glm::vec3 pos = idk::TransformSys::getWorldPosition(obj_id);

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
    mod.fun("playSound",  idk::AudioSys::playSound);
    mod.fun("pauseSound", idk::AudioSys::pauseSound);
    mod.fun("stopSound",  idk::AudioSys::stopSound);
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
    std::strcpy(cmp.filepath, filepath.data());
}



void
idk::AudioSys::playSound( int obj_id )
{
    auto &api   = getAPI();
    auto &ecs   = api.getECS();
    auto &audio = api.getAudioSys();

    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    audio.playSound(cmp.emitter_id);
}


void
idk::AudioSys::pauseSound( int obj_id )
{

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




