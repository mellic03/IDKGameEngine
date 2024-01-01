#include "idk_audio.hpp"

#include <libidk/idk_allocator.hpp>
#include <stack>


class idk::internal::AudioEngineAPI
{
public:
    static Allocator<Mix_Chunk *>                   mixchunks;
    static Allocator<idk::AudioEngine::Sound>       sounds;
    static Allocator<idk::AudioEngine::Emitter>     emitters;
    static std::stack<int>                          channels;
};

using namespace idk;
using EngineAPI = idk::internal::AudioEngineAPI;

Allocator<Mix_Chunk *>          EngineAPI::mixchunks = Allocator<Mix_Chunk *>();
Allocator<AudioEngine::Sound>   EngineAPI::sounds    = Allocator<AudioEngine::Sound>();
Allocator<AudioEngine::Emitter> EngineAPI::emitters  = Allocator<AudioEngine::Emitter>();
std::stack<int>                 EngineAPI::channels  = std::stack<int>();



idk::AudioEngine::AudioEngine()
{
    constexpr int AUDIOENGINE_NUM_CHANNELS = 8;
    for (int i=0; i<AUDIOENGINE_NUM_CHANNELS; i++)
    {
        EngineAPI::channels.push(i);
    }

    int audio_rate = 44100;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_buffers = 4096;

    #ifdef IDK_DEBUG
    if (Mix_OpenAudio(audio_rate, audio_format, AUDIOENGINE_NUM_CHANNELS, audio_buffers) != 0)
    {
        printf("Error initialising SDL2_audio\n");
        exit(1);
    }
    #endif
}


// int
// idk::AudioEngine::loadWav( std::string filepath )
// {
//     Mix_Chunk *mc = Mix_LoadWAV(filepath.c_str());

//     #ifdef IDK_DEBUG
//     if (mc == nullptr)
//     {
//         std::cout << "Could not load .wav file \"" << filepath << "\"\n";
//         exit(1);
//     }
//     #endif

//     return mixchunks.create(mc);
// }


// int
// idk::AudioEngine::createEmitter()
// {
//     return EngineAPI::m_emitter_allocator.create();
// }


int
idk::AudioEngine::createEmitter( int mix_chunk_id, const glm::vec3 &position )
{
    Mix_Chunk *mc = EngineAPI::mixchunks.get(mix_chunk_id);
    return EngineAPI::emitters.create(Emitter(mc, position));
}


// void
// idk::AudioEngine::listenerPosition( idk::Transform *transform )
// {
//     m_listener_transform = transform;
// }


// void
// idk::AudioEngine::playSound( int emitter_id )
// {
//     AudioEngine::Emitter emitter = m_emitter_allocator.get(emitter_id);
//     emitter.channel = channels.pop();
//     Mix_PlayChannel(emitter.channel, emitter.mc, -1);
// }


// void
// idk::AudioEngine::stopSound( int emitter_id )
// {
//     AudioEngine::Emitter emitter = m_emitter_allocator.get(emitter_id);

//     #ifdef IDK_DEBUG
//     if (emitter.channel == -1)
//     {
//         std::cout << "idk::AudioEngine::stopSound() emitter with channel -1 not allowed\n";
//         exit(1);
//     }
//     #endif

//     Mix_HaltChannel(emitter.channel);
//     channels.push(emitter.channel);
//     emitter.channel = -1;
// }



// void
// idk::AudioEngine::update()
// {
//     if (m_listener_transform == nullptr)
//         return;

//     glm::vec3 listener_pos = m_listener_transform->position();
//     glm::vec3 listener_dir = glm::normalize(this->m_listener_transform->front());

//     for (Emitter &emitter: m_emitter_allocator)
//     {
//         if (emitter.transform == nullptr)
//             continue;

//         glm::vec3 emitter_pos  = emitter.transform->position();
//         glm::vec3 dir = glm::normalize(emitter_pos - listener_pos);
        
//         float dot = glm::dot(dir, listener_dir);
//         dot = (dot + 1.0f) / 2.0f;

//         Mix_Volume(emitter.channel, dot*SDL_MIX_MAXVOLUME);
//     }
// }

