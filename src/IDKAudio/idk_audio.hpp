#pragma once

#include <libidk/idk_transform.hpp>
#include <SDL2/SDL_mixer.h>



namespace idk { class AudioSystem; };


class idk::AudioSystem
{
public:
    struct Sound
    {
        Mix_Chunk *mc;
        const char *name;
    };

    struct Emitter
    {
        int channel;
        const Mix_Chunk *mc;
        const glm::vec3 &position;

        Emitter ( const Mix_Chunk *mix, const glm::vec3 &pos )
        : channel(-1),  mc(mix),  position(pos) {  };
    
        Emitter( const Emitter &rhs )
        : channel(rhs.channel),  mc(rhs.mc),  position(rhs.position) {  };

        Emitter( const Emitter &&rhs )
        : channel(rhs.channel),  mc(rhs.mc),  position(std::move(rhs.position)) {  };

    };

private:


public:
                // AudioSystem();

    // void        update();
    // int         loadWav( const char * );
    // int         createEmitter();
    // int         createEmitter( int mix_chunk_id, const glm::vec3 &position );

    // void        listenerPosition( idk::Transform * );

    // void        playSound( int emitter_id );
    // void        stopSound( int emitter_id );

};


