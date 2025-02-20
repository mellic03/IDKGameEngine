#include "idk_audio.hpp"
#include <libidk/idk_log2.hpp>

#include <filesystem>
#include <map>


namespace
{
    static constexpr int NUM_CHANNELS = 32;
    // static int m_curr_channel = 0;

    static std::stack<int>                           m_channels;
    static std::map<std::string, int>                m_wav_cache;

    static idk::Allocator<Mix_Chunk>                 m_chunks;
    static idk::Allocator<idk::AudioSystem::Emitter> m_emitters;

};



void
idk::AudioSystem::init()
{
    // for (int i=0; i<NUM_CHANNELS; i++)
    // {
    //     m_channels.push(i);
    // }

    int    audio_rate    = 44100;
    Uint16 audio_format  = MIX_DEFAULT_FORMAT; // AUDIO_S16SYS;
    int    audio_buffers = 2048;


    IDK_ASSERT(
        "Error initialising SDL2 audio",
        Mix_OpenAudio(audio_rate, audio_format, 1, audio_buffers) == 0
    );

    IDK_ASSERT(
        "Error on Mix_AllocateChannels",
        Mix_AllocateChannels(NUM_CHANNELS) == NUM_CHANNELS
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


int
idk::AudioSystem::createEmitter( const std::string &filepath )
{
    int sound = loadWav(filepath);
    int id = m_emitters.create(Emitter(sound));
    auto &em = AudioSystem::getEmitter(id);
    em.id  = id;
    em.att = glm::vec4(1, 0, 0, 0);

    return id;
}



idk::AudioSystem::Emitter&
idk::AudioSystem::getEmitter( int id )
{
    return m_emitters.get(id);
}


void
idk::AudioSystem::destroyEmitter( int id )
{
    if (m_emitters.contains(id))
    {
        m_emitters.destroy(id);
    }
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
        LOG_INFO("AudioSystem::loadWav", ".wav file already cached: " + filepath);
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

    if (emitter.chunk == -1)
    {
        return;
    }

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


    // IDK_ASSERT("m_channels is empty", m_channels.empty() == false);

    // if (m_channels.empty())
    // {
    //     return;
    // }

    // m_curr_channel = (m_curr_channel + 1) % NUM_CHANNELS;
    // emitter.channel = m_curr_channel;

    // emitter.channel = m_channels.top();
    //                   m_channels.pop();

    Mix_Chunk &chunk = getChunk(emitter.chunk);
    emitter.channel = Mix_PlayChannel(-1, &chunk, (loop) ? -1 : 0);
    std::cout << "channel: " << emitter.channel << "\n";
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
    // m_channels.push(emitter.channel);

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
        float dist2 = glm::distance2(emitter.pos, listener_pos);

        float mag  = 1.0f / (1.0f + emitter.att[1]*sqrt(dist2) + emitter.att[2]*dist2);
              mag *= emitter.volume;
              mag  = glm::clamp(mag, 0.0f, 1.0f);

        int volume = int(mag * float(SDL_MIX_MAXVOLUME));
            volume = glm::clamp(volume, 0, SDL_MIX_MAXVOLUME);

        // idk_printvalue(sqrt(dist2));
        // idk_printvalue(volume);
        // idk_printvalue(emitter.channel);
        // std::cout << "\n";

        Mix_Volume(emitter.channel, volume);

        if (Mix_Playing(emitter.channel) == 0)
        {
            stopSound(emitter.id);
        }
    }
    // std::cout << "\n";

}
