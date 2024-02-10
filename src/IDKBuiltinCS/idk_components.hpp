#pragma once

#include <IDKECS/IDKECS.hpp>
#include <IDKAudio/IDKAudio.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>

#include "../../external/include/idk_icons/idk_Icons.hpp"
#include <libidk/idk_transform.hpp>
#include <libidk/idk_scripting.hpp>


namespace idk
{
    struct IconCmp
    {
        int  obj_id = -1;
        char icon[32];

        IconCmp( int obj = -1, IconCmp *other = nullptr, idecs::ECS *ecs = nullptr )
        : obj_id(obj)
         {
            std::string str = ICON_FA_CUBE;
            std::strcpy(icon, str.c_str());

            if (other != nullptr)
            {
                std::strcpy(icon, other->icon);
            }
        };
    };


    struct TransformCmp
    {
        int       obj_id   = -1;
        glm::vec3 position = glm::vec3(0.0f);
        glm::quat rotation = glm::quat();
        glm::vec3 scale    = glm::vec3(1.0f);

        TransformCmp( int id = -1, TransformCmp *other = nullptr, idecs::ECS *ecs = nullptr )
        : obj_id(id)
        {
            if (other)
            {
                *this = *other;
                obj_id = id;
            }
        };
    };

    struct PhysicsMotionCmp
    {
        int       obj_id   = -1;

        float     lin_drag     = 1.0f;
        glm::vec3 lin_velocity = glm::vec3(0.0f);

        float     ang_drag     = 1.0f;
        float     ang_velocity = 0.0f;
        glm::vec3 ang_axis     = glm::vec3(0.0f, 1.0f, 0.0f);


        PhysicsMotionCmp( int obj = -1, PhysicsMotionCmp *other = nullptr, idecs::ECS *ecs = nullptr )
        : obj_id(obj)
        {
            if (other)
            {
                *this = *other;
                obj_id = obj;
            }
        };
    };


    struct BoxColliderCmp
    {
        int  obj_id;
        bool visualize;

        BoxColliderCmp( int obj = -1, BoxColliderCmp *other = nullptr, idecs::ECS *ecs = nullptr )
        : obj_id(obj)
        {
            
        };
    };


    struct SphereColliderCmp
    {
        int  obj_id;
        bool visualize;

        SphereColliderCmp( int obj = -1, SphereColliderCmp *other = nullptr, idecs::ECS *ecs = nullptr )
        : obj_id(obj)
        {

        };
    };


    struct ScriptCmp
    {
        int obj_id = -1;
    
        lua_State *L = idk::LuaManager::newState();
        int  nparams;
        int  subject_id = -1;
        int  target_id  = -1;
        int  dependency = -1;
        int  retvalue   = -1;
        char filepath[128];

        ScriptCmp( int obj = -1, ScriptCmp *other = nullptr, idecs::ECS *ecs = nullptr )
        : obj_id(obj)
        {
            if (ecs)
            {
                ecs->giveComponent<TransformCmp>(obj);
            }

            std::fill_n(filepath, 128, '\0');
        
            if (other == nullptr)
            {
                return;
            }

            L = idk::LuaManager::newState();
            nparams     = other->nparams;
            subject_id  = other->subject_id;
            target_id   = other->target_id;
            dependency  = other->dependency;
            retvalue    = other->retvalue;
            std::strcpy(filepath, other->filepath);
        };

        ~ScriptCmp()
        {
            // lua_close(L);
        };

    };

    int ScriptCmp_getNumParams( idk::ScriptCmp & );


    struct ModelCmp
    {
        int  obj_id      = -1;
        int  model_id    = -1;
        bool visible     = true;
        bool shadowcast  = true;
        char filepath[128];
        char filestem[128];

        ModelCmp( int obj = -1, ModelCmp *other = nullptr, idecs::ECS *ecs = nullptr )
        : obj_id(obj)
        {
            if (ecs && ecs->hasComponent<idk::TransformCmp>(obj) == false)
            {
                ecs->giveComponent<idk::TransformCmp>(obj);
            }

            std::fill_n(filepath, 128, '\0');
            std::fill_n(filestem, 128, '\0');

            if (other == nullptr)
            {
                return;
            }

            model_id    = other->model_id;
            visible     = other->visible;
            shadowcast  = other->shadowcast;
            std::strcpy(filepath, other->filepath);
            std::strcpy(filestem, other->filestem);
        };
    };


    struct CameraCmp
    {
        int obj_id = -1;
        int cam_id = -1;

        CameraCmp( int obj = -1, CameraCmp *other = nullptr, idecs::ECS *ecs = nullptr )
        : obj_id(obj)
        {

        };
    };


    struct AudioEmitterCmp
    {
        int   obj_id     = -1;
        int   emitter_id = -1;
        float volume     = 0.5f;
        char  filepath[128];


        AudioEmitterCmp( int obj = -1, AudioEmitterCmp *other = nullptr, idecs::ECS *ecs = nullptr )
        : obj_id(obj)
        {
            std::fill_n(filepath, 128, '\0');

            if (ecs)
            {
                ecs->giveComponent<TransformCmp>(obj);
            }

            if (other)
            {
                emitter_id = other->emitter_id;
                volume     = other->volume;
                std::strcpy(filepath, other->filepath);
            }
        };
    };


    struct AudioListenerCmp
    {
        int obj_id = -1;

        AudioListenerCmp( int obj = -1, AudioListenerCmp *other = nullptr, idecs::ECS *ecs = nullptr )
        : obj_id(obj)
        {
            if (ecs)
            {
                ecs->giveComponent<TransformCmp>(obj);
            }
        };
    };



    // template <typename T>
    // idecs::Behaviour getBehaviour(); 


};




