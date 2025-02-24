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

        static void onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
        {
            // this->obj_id = obj_id;
        };

        static void onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id ) {  };
        static void onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj ) {  };

    };


    struct CameraCmp
    {
        int   obj_id   = -1;
        int   cam_id   = -1;

        IDK_Camera camera;

        size_t serialize( std::ofstream &stream ) const;
        size_t deserialize( std::ifstream &stream );
        static void onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
        static void onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
        static void onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj );
    };


};


