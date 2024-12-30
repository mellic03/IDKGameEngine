#pragma once

#include <IDKECS/IDKECS.hpp>
#include <IDKAudio/IDKAudio.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>

#include <idk_icons/idk_Icons.hpp>
// #include "../../external/include/idk_icons/idk_Icons.hpp"
#include <libidk/idk_serialize.hpp>



namespace idk
{
    struct IconCmp
    {
        #define IDK_STRUCT_MEMBERS(X) \
            X( int, obj_id, -1 ) \
            X( std::string, icon, ICON_FA_CUBE )

        IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
        #undef IDK_STRUCT_MEMBERS

        static void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            // this->obj_id = obj_id;
        };

        static void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
        static void onObjectCopy( int src_obj, int dst_obj ) {  };

    };


    struct CameraCmp
    {
        int   obj_id   = -1;
        int   cam_id   = -1;

        IDK_Camera camera;

        size_t serialize( std::ofstream &stream ) const;
        size_t deserialize( std::ifstream &stream );
        static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
        static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
        static void onObjectCopy( int src_obj, int dst_obj );
    };


    struct AudioEmitterCmp
    {
        int         obj_id     = -1;
        int         emitter_id = -1;
        float       volume     = -1;
        std::string filepath   = "";
    
        size_t serialize( std::ofstream &stream ) const
        {
            size_t n = 0;
            n += idk::streamwrite(stream, obj_id);
            n += idk::streamwrite(stream, emitter_id);
            n += idk::streamwrite(stream, volume);

            std::string str = std::filesystem::relative(filepath);
            idk_printvalue(str);

            n += idk::streamwrite(stream, str);
            return n;
        };

        size_t deserialize( std::ifstream &stream )
        {
            size_t n = 0;
            n += idk::streamread(stream, obj_id);
            n += idk::streamread(stream, emitter_id);
            n += idk::streamread(stream, volume);
            n += idk::streamread(stream, filepath);
            return n;
        };

        static void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            // auto &cmp = ECS2::getComponent<AudioEmitterCmp>(obj_id);
            // cmp.emitter_id = AudioSystem::createEmitter();
            // this->obj_id = obj_id;
        };
    
        static void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
        {
            auto &cmp = ECS2::getComponent<AudioEmitterCmp>(obj_id);
        
            if (cmp.emitter_id != -1)
            {
                AudioSystem::destroyEmitter(cmp.emitter_id);
                cmp.emitter_id = -1;
            }
        };

        static void onObjectCopy( int src_obj, int dst_obj )
        {

        };

    };


    struct AudioListenerCmp
    {
        #define IDK_STRUCT_MEMBERS(X) \
            X( int, obj_id, -1 ) \
    
        IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
        #undef IDK_STRUCT_MEMBERS

        static void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            // this->obj_id = obj_id;
        };

        static void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
        {

        };

        static void onObjectCopy( int src_obj, int dst_obj ) {  };

    };

};


