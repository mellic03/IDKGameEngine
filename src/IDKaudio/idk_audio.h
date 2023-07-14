#pragma once

#include "IDKcommon/IDKcommon.h"

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
    Allocator<Mix_Chunk *>  _mixchunk_allocator;
    Allocator<Sound>        _sound_allocator;
    Allocator<Emitter>      _emitter_allocator;
    Transform *             _listener_position;
    idk::stack<int>         _audio_channels;


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


