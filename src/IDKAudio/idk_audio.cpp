#include "idk_audio.hpp"


idk::AudioSystem::AudioSystem()
{
    static constexpr int AUDIOENGINE_NUM_CHANNELS = 8;

    for (int i=0; i<AUDIOENGINE_NUM_CHANNELS; i++)
    {
        m_channels.push(i);
    }

    int    audio_rate    = 44100;
    Uint16 audio_format  = AUDIO_S16SYS;
    int    audio_buffers = 4096;

    IDK_ASSERT(
        "Error initialising SDL2 audio",
        Mix_OpenAudio(audio_rate, audio_format, AUDIOENGINE_NUM_CHANNELS, audio_buffers) == 0
    );

}


int
idk::AudioSystem::loadWav( const std::string &filepath )
{
    Mix_Chunk *mc = Mix_LoadWAV(filepath.c_str());
    IDK_ASSERT("Could not load .wav file", mc != nullptr);

    return createChunk(*mc);
}


void
idk::AudioSystem::playSound( int emitter_id )
{
    auto &emitter = getEmitter(emitter_id);

    // Already playing
    if (emitter.channel != -1)
    {
        return;
    }


    IDK_ASSERT("m_channels is empty", m_channels.empty() == false);

    if (m_channels.empty())
    {
        return;
    }

    emitter.channel = m_channels.top();
                      m_channels.pop();

    Mix_Chunk &chunk = getChunk(emitter.chunk);
    Mix_PlayChannel(emitter.channel, &chunk, -1);
}


void
idk::AudioSystem::stopSound( int emitter_id )
{
    auto &emitter = getEmitter(emitter_id);

    // Already stopped
    if (emitter.channel == -1)
    {
        return;
    }

    Mix_HaltChannel(emitter.channel);
    m_channels.push(emitter.channel);
    emitter.channel = -1;
}



void
idk::AudioSystem::update( const glm::vec3 &listener_pos, const glm::vec3 &listener_front )
{
    std::cout << m_emitters.size() << "\n";

    for (Emitter &emitter: m_emitters)
    {
        glm::vec3 dir = glm::normalize(emitter.pos - listener_pos);
        float     mag = glm::dot(dir, listener_front) * 0.5 + 0.5;

        idk_printvalue(mag);

        Mix_Volume(emitter.channel, mag*SDL_MIX_MAXVOLUME);
    }

}
