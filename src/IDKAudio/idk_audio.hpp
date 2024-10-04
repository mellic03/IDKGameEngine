#pragma once

#include <SDL2/SDL_mixer.h>

#include <libidk/idk_allocator.hpp>
#include <libidk/idk_glm.hpp>

#include <string>
#include <stack>


namespace idk { class AudioSystem; };



class idk::AudioSystem
{
public:

    struct Emitter
    {
        int   id       = -1;
        int   channel  = -1;
        int   chunk    = -1;
        bool  looping  = false;
        float volume   = 1.0f;
        int   next     = -1;
        glm::vec3 pos = glm::vec3(0.0f);
        glm::vec3 att = glm::vec3(1.0f, 0.01f, 0.0f);

        Emitter() {  };
        Emitter( int chunk_id ): chunk(chunk_id) {  };
    };


private:
    static int createChunk( const Mix_Chunk& );
    static Mix_Chunk &getChunk( int );

public:
                // AudioSystem();

    static void init();
    static void update( const glm::vec3&, const glm::vec3 & );
    static int  loadWav( const std::string & );

    static int createEmitter();
    static int createEmitter( const Emitter& );
    static int createEmitter( const std::string& );
    static Emitter &getEmitter( int );
    static void destroyEmitter( int );

    // IDK_ALLOCATOR_ACCESS(Emitter, Emitter,   m_emitters)
    // IDK_ALLOCATOR_ACCESS(Chunk,   Mix_Chunk, m_chunks)

    static void playSound( int emitter_id, bool loop );
    static void stopSound( int emitter_id );

    static void resumeSound( int emitter_id );
    static void pauseSound( int emitter_id );

};


