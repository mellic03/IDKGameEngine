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
    
    // for (auto &[obj_id, emitter_id, volume, filepath]: ecs.getComponentArray<AudioEmitterCmp>())
    // {
    //     glm::vec3 pos = idk::TransformSys::getPositionWorldspace(obj_id);

    //     if (obj_id == -1 || emitter_id == -1)
    //     {
    //         continue;
    //     }

    //     int sound = loadSound(filepath);
    //     emitter_id = AudioSystem::createEmitter(idk::AudioSystem::Emitter(sound));
    //     AudioSystem::getEmitter(emitter_id).id = emitter_id;
    // }
}


void
idk::AudioSys::update( idk::EngineAPI &api )
{
    auto &ecs  = getECS();
    auto &tsys = ecs.getSystem<TransformSys>();
    // static std::set<int> finished_callbacks;
    // finished_callbacks.clear();

    // for (auto &[obj_id, callback]: m_callbacks)
    // {
    //     if (isFinished(obj_id))
    //     {
    //         callback();
    //         finished_callbacks.insert(obj_id);
    //     }
    // }

    // for (int obj_id: finished_callbacks)
    // {
    //     m_callbacks.erase(obj_id);
    // }

    for (auto &cmp: ecs.getComponentArray<AudioEmitterCmp>())
    {
        glm::vec3 pos = tsys.getWorldPosition(cmp.obj_id);
        AudioSystem::getEmitter(cmp.emitter_id).pos = pos;
    }



    for (auto &[obj_id]: ecs.getComponentArray<AudioListenerCmp>())
    {
        glm::vec3 pos = tsys.getWorldPosition(obj_id);
        glm::vec3 dir = tsys.getFront(obj_id);
        // std::cout << pos.x << ", " << pos.y << ", " << pos.z << "\n";

        AudioSystem::update(pos, dir);
    }

    // std::cout << "\n";
}



int
idk::AudioSys::loadSound( const std::string &filepath )
{
    return AudioSystem::loadWav(filepath);
}


void
idk::AudioSys::assignSound( int obj_id, int sound )
{
    auto &ecs = api_ptr->getECS();
    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);

    if (cmp.emitter_id != -1)
    {
        auto &emm = AudioSystem::getEmitter(cmp.emitter_id);
        emm.chunk = sound;
    }

    else
    {
        cmp.emitter_id = AudioSystem::createEmitter(sound);
    }

}

void
idk::AudioSys::assignSound( int obj_id, const std::string &filepath )
{
    int sound = loadSound(filepath);
    assignSound(obj_id, sound);

    auto &ecs = api_ptr->getECS();
    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    // cmp.emitter_id = AudioSystem::createEmitter(idk::AudioSystem::Emitter(sound));
    cmp.filepath = filepath;
}


// void
// idk::AudioSys::playSoundCallback( int obj_id, std::function<void()> callback )
// {
//     auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
//     m_callbacks[obj_id] = callback;
//     AudioSystem::playSound(cmp.emitter_id, false);
// }


void
idk::AudioSys::playSound( int obj_id, bool loop )
{
    auto &ecs = api_ptr->getECS();
    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    AudioSystem::playSound(cmp.emitter_id, loop);
}


void
idk::AudioSys::stopSound( int obj_id )
{
    auto &ecs = api_ptr->getECS();
    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    AudioSystem::stopSound(cmp.emitter_id);
}


void
idk::AudioSys::resumeSound( int obj_id )
{
    auto &ecs = api_ptr->getECS();
    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    AudioSystem::resumeSound(cmp.emitter_id);
}


void
idk::AudioSys::pauseSound( int obj_id )
{
    auto &ecs = api_ptr->getECS();
    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    AudioSystem::pauseSound(cmp.emitter_id);
}


void
idk::AudioSys::setVolume( int obj_id, float volume )
{
    auto &ecs = api_ptr->getECS();
    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    auto &emm = AudioSystem::getEmitter(cmp.emitter_id);
    emm.volume = volume;
}




bool
idk::AudioSys::isFinished( int obj_id )
{
    auto &ecs = api_ptr->getECS();
    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    auto &emm = AudioSystem::getEmitter(cmp.emitter_id);

    return emm.channel == -1;
}


bool
idk::AudioSys::isPlaying( int obj_id )
{
    auto &ecs = api_ptr->getECS();
    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);
    return AudioSystem::getEmitter(cmp.emitter_id).channel != -1;
}







size_t
idk::AudioEmitterCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, emitter_id);
    n += idk::streamwrite(stream, volume);

    std::string str = std::filesystem::relative(filepath);
    idk_printvalue(str);

    n += idk::streamwrite(stream, str);
    return n;
}


size_t
idk::AudioEmitterCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, emitter_id);
    n += idk::streamread(stream, volume);
    n += idk::streamread(stream, filepath);
    return n;
}


void
idk::AudioEmitterCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    // auto &cmp = ECS2::getComponent<AudioEmitterCmp>(obj_id);
    // cmp.emitter_id = AudioSystem::createEmitter();
    // this->obj_id = obj_id;
}


void
idk::AudioEmitterCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<AudioEmitterCmp>(obj_id);

    if (cmp.emitter_id != -1)
    {
        AudioSystem::destroyEmitter(cmp.emitter_id);
        cmp.emitter_id = -1;
    }
}


void
idk::AudioEmitterCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
{

}


