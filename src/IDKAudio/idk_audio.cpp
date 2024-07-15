#include "idk_audio.hpp"

#include <filesystem>


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
    std::string path = std::filesystem::absolute(filepath);

    Mix_Chunk *mc = Mix_LoadWAV(path.c_str());
    IDK_ASSERT("Could not load .wav file", mc != nullptr);

    idk_printvalue(mc);

    return createChunk(*mc);
}


void
idk::AudioSystem::playSound( int emitter_id, bool loop )
{
    auto &emitter = getEmitter(emitter_id);
    emitter.looping = loop;

    // Already playing
    if (emitter.channel != -1)
    {
        stopSound(emitter_id);
        playSound(emitter_id, loop);
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
    Mix_PlayChannel(emitter.channel, &chunk, (loop) ? -1 : 0);
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
idk::AudioSystem::resumeSound( int emitter_id )
{
    auto &emitter = getEmitter(emitter_id);

    if (emitter.channel == -1)
    {
        playSound(emitter_id, emitter.looping);
        return;
    }

    if (Mix_Paused(emitter.channel))
    {
        Mix_Resume(emitter.channel);
    }
}


void
idk::AudioSystem::pauseSound( int emitter_id )
{
    auto &emitter = getEmitter(emitter_id);

    if (emitter.channel == -1)
    {
        return;
    }

    if (Mix_Paused(emitter.channel) == false)
    {
        Mix_Pause(emitter.channel);
    }
}



void
idk::AudioSystem::update( const glm::vec3 &listener_pos, const glm::vec3 &listener_front )
{
    for (Emitter &emitter: m_emitters)
    {
        glm::vec3 dir  = glm::normalize(emitter.pos - listener_pos);
        float     dist = glm::distance(emitter.pos, listener_pos);

        float mag = glm::dot(listener_front, dir) * 0.5f + 0.5f;
              mag /= (0.1f * dist);
              mag = glm::clamp(mag, 0.0f, 1.0f);

        Mix_Volume(emitter.channel, mag*SDL_MIX_MAXVOLUME);

        if (emitter.looping == false && Mix_Playing(emitter.channel) == 0)
        {
            stopSound(emitter.id);
        }

    }

}
