#include "idk_audio.hpp"

#include <libidk/idk_log.hpp>

#include <filesystem>
#include <map>


namespace
{
    std::stack<int>                           m_channels;
    std::map<std::string, int>                m_wav_cache;

    idk::Allocator<Mix_Chunk>                 m_chunks;
    idk::Allocator<idk::AudioSystem::Emitter> m_emitters;

};



void
idk::AudioSystem::init()
{
    static constexpr int NUM_CHANNELS = 8;

    for (int i=0; i<NUM_CHANNELS; i++)
    {
        m_channels.push(i);
    }

    int    audio_rate    = 44100;
    Uint16 audio_format  = AUDIO_S16SYS;
    int    audio_buffers = 4096;

    IDK_ASSERT(
        "Error initialising SDL2 audio",
        Mix_OpenAudio(audio_rate, audio_format, NUM_CHANNELS, audio_buffers) == 0
    );
}



int
idk::AudioSystem::createEmitter()
{
    return m_emitters.create();
}


int
idk::AudioSystem::createEmitter( const Emitter &emitter )
{
    int id = m_emitters.create(emitter);
    AudioSystem::getEmitter(id).id = id;
    return id;
}


idk::AudioSystem::Emitter&
idk::AudioSystem::getEmitter( int id )
{
    return m_emitters.get(id);
}


int
idk::AudioSystem::createChunk( const Mix_Chunk &chunk )
{
    return m_chunks.create(chunk);
}


Mix_Chunk&
idk::AudioSystem::getChunk( int id )
{
    return m_chunks.get(id);
}





int
idk::AudioSystem::loadWav( const std::string &filepath )
{
    std::string path = std::filesystem::absolute(filepath);

    if (m_wav_cache.contains(path))
    {
        LOG_INFO() << "[AudioSystem] .wav file already cached :" << filepath;
        return m_wav_cache[path];
    }

    Mix_Chunk *mc = Mix_LoadWAV(path.c_str());

    std::string msg  = "[idk::AudioSystem]";
                msg += " Could not load file \"" + filepath + "\"";

    IDK_ASSERT(msg.c_str(), mc != nullptr);

    int id = createChunk(*mc);
    m_wav_cache[path] = id;

    return id;
}


void
idk::AudioSystem::playSound( int emitter_id, bool loop )
{
    auto &emitter = getEmitter(emitter_id);
    emitter.looping = loop;

    // Already playing
    if (emitter.channel != -1)
    {
        if (loop == true)
        {
            return;
        }

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
