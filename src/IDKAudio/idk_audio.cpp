#include "idk_audio.hpp"


idk::Audio::Audio( idk::linear_allocator *mainblock )
:   m_chunks   (512, mainblock),
    m_emitters (512, mainblock)
{

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
idk::Audio::createEmitter()
{
    return m_emitters.create();
}


int
idk::Audio::createEmitter( const Emitter &emitter )
{
    int id = m_emitters.create(emitter);
    Audio::getEmitter(id).id = id;
    return id;
}


int
idk::Audio::createEmitter( const std::string &filepath )
{
    int sound = loadWav(filepath);
    int id = m_emitters.create(Emitter(sound));
    auto &em = Audio::getEmitter(id);
    em.id  = id;
    em.att = glm::vec4(1, 0, 0, 0);

    return id;
}



idk::Audio::Emitter&
idk::Audio::getEmitter( int id )
{
    return m_emitters.get(id);
}


void
idk::Audio::destroyEmitter( int id )
{
    // if (m_emitters.contains(id))
    {
        m_emitters.destroy(id);
    }
}



int
idk::Audio::createChunk( const Mix_Chunk &chunk )
{
    return m_chunks.create(chunk);
}


Mix_Chunk&
idk::Audio::getChunk( int id )
{
    return m_chunks.get(id);
}





int
idk::Audio::loadWav( const std::string &filepath )
{
    std::string path = std::filesystem::absolute(filepath);

    if (m_wav_cache.contains(path))
    {
        LOG_INFO(".wav file already cached: \"{}\"", filepath);
        return m_wav_cache[path];
    }

    Mix_Chunk *mc = Mix_LoadWAV(path.c_str());

    std::string msg  = "[idk::Audio]";
                msg += " Could not load file \"" + filepath + "\"";

    IDK_ASSERT(msg.c_str(), mc != nullptr);

    int id = createChunk(*mc);
    m_wav_cache[path] = id;

    return id;
}


void
idk::Audio::playSound( int emitter_id, bool loop )
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

    LOG_ADV(
        log_flag::AUDIO | log_flag::DETAIL,
        "Playing emitter {} on channel {}", emitter.id, emitter.channel
    );
}


void
idk::Audio::stopSound( int emitter_id )
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
idk::Audio::resumeSound( int emitter_id )
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
idk::Audio::pauseSound( int emitter_id )
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
idk::Audio::update( const glm::vec3 &listener_pos, const glm::vec3 &listener_front )
{
    for (Emitter &emitter: m_emitters)
    {
        float dist2 = glm::distance2(emitter.pos, listener_pos);

        float mag  = 1.0f / (1.0f + emitter.att[1]*sqrt(dist2) + emitter.att[2]*dist2);
              mag *= emitter.volume;
              mag  = glm::clamp(mag, 0.0f, 1.0f);

        int volume = int(mag * float(SDL_MIX_MAXVOLUME));
            volume = glm::clamp(volume, 0, SDL_MIX_MAXVOLUME);

        Mix_Volume(emitter.channel, volume);

        if (Mix_Playing(emitter.channel) == 0)
        {
            stopSound(emitter.id);
        }
    }
}
