#pragma once

#include "libidk/libidk.hpp"
#include <SDL2/SDL_mixer.h>

namespace idk { class AudioEngine; };


class idk::AudioEngine
{
public:
    struct Sound
    {
        Mix_Chunk *mc;
        std::string name;
    };

    struct Emitter
    {
        int channel;
        Mix_Chunk *mc;
        idk::Transform *transform;
    };


private:
    Allocator<Mix_Chunk *>  m_mixchunk_allocator;
    Allocator<Sound>        m_sound_allocator;
    Allocator<Emitter>      m_emitter_allocator;
    Transform *             m_listener_transform;
    idk::stack<int>         m_audio_channels;


public:
                AudioEngine();

    void        update();

    int         loadWav( std::string );
    
    int         createEmitter();
    int         createEmitter( int mix_chunk_id, idk::Transform & );

    void        listenerPosition( idk::Transform * );

    void        playSound( int emitter_id );
    void        stopSound( int emitter_id );

};


