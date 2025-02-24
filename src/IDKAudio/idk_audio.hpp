#pragma once

#include <SDL2/SDL_mixer.h>

#include <libidk/idk_allocator.hpp>
#include <libidk/idk_glm.hpp>
#include <libidk/idk_log.hpp>
#include <libidk/memory/linear_allocator.hpp>

#include <string>
#include <map>
#include <stack>
#include <filesystem>

namespace idk { class Audio; };


class idk::Audio
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
    static constexpr int NUM_CHANNELS = 32;

    std::stack<int>                 m_channels;
    std::map<std::string, int>      m_wav_cache;
    idk::Allocator<Mix_Chunk>       m_chunks;
    idk::Allocator<Emitter>         m_emitters;
    // idk::pool_allocator<Mix_Chunk>  m_chunks;
    // idk::pool_allocator<Emitter>    m_emitters;

    int createChunk( const Mix_Chunk& );
    Mix_Chunk &getChunk( int );

public:
    Audio( idk::linear_allocator *mainblock );

    void update( const glm::vec3&, const glm::vec3 & );
    int  loadWav( const std::string & );

    int createEmitter();
    int createEmitter( const Emitter& );
    int createEmitter( const std::string& );
    Emitter &getEmitter( int );
    void destroyEmitter( int );

    // IDK_ALLOCATOR_ACCESS(Emitter, Emitter,   m_emitters)
    // IDK_ALLOCATOR_ACCESS(Chunk,   Mix_Chunk, m_chunks)

    void playSound( int emitter_id, bool loop );
    void stopSound( int emitter_id );

    void resumeSound( int emitter_id );
    void pauseSound( int emitter_id );

};


