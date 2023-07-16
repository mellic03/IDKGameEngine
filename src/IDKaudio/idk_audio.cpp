#include "idk_audio.h"


idk::AudioEngine::AudioEngine()
{
    constexpr int AUDIOENGINE_NUM_CHANNELS = 8;
    for (int i=0; i<AUDIOENGINE_NUM_CHANNELS; i++)
    {
        m_audio_channels.push(i);
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


int
idk::AudioEngine::loadWav( std::string filepath )
{
    Mix_Chunk *mc = Mix_LoadWAV(filepath.c_str());

    #ifdef IDK_DEBUG
    if (mc == nullptr)
    {
        std::cout << "Could not load .wav file \"" << filepath << "\"\n";
        exit(1);
    }
    #endif

    return m_mixchunk_allocator.add(mc);
}


int
idk::AudioEngine::createEmitter()
{
    return m_emitter_allocator.add();
}


int
idk::AudioEngine::createEmitter( int mix_chunk_id, idk::Transform &transform )
{
    Mix_Chunk *mc = m_mixchunk_allocator.get(mix_chunk_id);
    return m_emitter_allocator.add(  { -1, mc, &transform }  );
}


void
idk::AudioEngine::listenerPosition( idk::Transform *transform )
{
    m_listener_transform = transform;
}


void
idk::AudioEngine::playSound( int emitter_id )
{
    AudioEngine::Emitter emitter = m_emitter_allocator.get(emitter_id);
    emitter.channel = m_audio_channels.pop();
    Mix_PlayChannel(emitter.channel, emitter.mc, -1);
}


void
idk::AudioEngine::stopSound( int emitter_id )
{
    AudioEngine::Emitter emitter = m_emitter_allocator.get(emitter_id);

    #ifdef IDK_DEBUG
    if (emitter.channel == -1)
    {
        std::cout << "idk::AudioEngine::stopSound() emitter with channel -1 not allowed\n";
        exit(1);
    }
    #endif

    Mix_HaltChannel(emitter.channel);
    m_audio_channels.push(emitter.channel);
    emitter.channel = -1;
}



void
idk::AudioEngine::update()
{
    this->m_emitter_allocator.for_each(
        [*this](AudioEngine::Emitter &emitter)
        {

        }
    );
}

