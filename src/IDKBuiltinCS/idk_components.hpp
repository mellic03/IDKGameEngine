#pragma once

#include <IDKECS/IDKECS.hpp>
#include <IDKAudio/IDKAudio.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>

#include "../../external/include/idk_icons/idk_Icons.hpp"
#include <libidk/idk_scripting.hpp>
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

        void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            this->obj_id = obj_id;
        };

        void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
        void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj ) {  };

    };


    struct DirlightCmp
    {
        #define IDK_STRUCT_MEMBERS(X) \
            X( int, obj_id,   -1 ) \
            X( int, light_id, -1 ) \
            X( glm::vec4, diffuse, glm::vec4(1.0f) ) \
            X( glm::vec4, ambient, glm::vec4(0.1f) )
    
        IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
        #undef IDK_STRUCT_MEMBERS

        void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            this->obj_id = obj_id;
            light_id = api.getRenderer().createDirlight();
        };

        void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
        void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj ) {  };

    };

    struct PointlightCmp
    {
        #define IDK_STRUCT_MEMBERS(X) \
            X( int,       obj_id,    -1 ) \
            X( int,       light_id,  -1 ) \
            X( glm::vec4, diffuse,   glm::vec4(1.0f) ) \
            X( float,     radius,    16.0f )
    
        IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
        #undef IDK_STRUCT_MEMBERS

        void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            this->obj_id = obj_id;
            light_id = api.getRenderer().createPointlight();
        };

        void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
        {
            api.getRenderer().destroyPointlight(light_id);
        };

        void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj ) {  };

    };

    struct SpotlightCmp
    {
        #define IDK_STRUCT_MEMBERS(X) \
            X( int,       obj_id,    -1 ) \
            X( int,       light_id,  -1 ) \
            X( glm::vec4, diffuse,   glm::vec4(1.0f) ) \
            X( glm::vec3, angle,     glm::vec3(0.8f, 0.7f, 0.0f) ) \
            X( float,     radius,    16.0f )
    
        IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
        #undef IDK_STRUCT_MEMBERS

        void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            this->obj_id = obj_id;
            light_id = api.getRenderer().createSpotlight();
        };
    
        void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
        void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj ) {  };

    };


    struct ScriptCmp
    {
        int         obj_id     = -1;
        bool        enabled    = false;
        lua_State  *L          = nullptr;
        int         nparams    = -1;
        int         subject_id = -1;
        int         target_id  = -1;
        int         dependency = -1;
        int         retvalue   = -1;
        std::string filepath   = "";


        size_t serialize( std::ofstream &stream ) const
        {
            size_t n = 0;
            n += idk::streamwrite(stream, obj_id);
            n += idk::streamwrite(stream, enabled);
            n += idk::streamwrite(stream, L);
            n += idk::streamwrite(stream, nparams);
            n += idk::streamwrite(stream, subject_id);
            n += idk::streamwrite(stream, target_id);
            n += idk::streamwrite(stream, dependency);
            n += idk::streamwrite(stream, retvalue);
            n += idk::streamwrite(stream, filepath);
            return n;
        };

        size_t deserialize( std::ifstream &stream )
        {
            size_t n = 0;
            n += idk::streamread(stream, obj_id);
            n += idk::streamread(stream, enabled);
            n += idk::streamread(stream, L);
            n += idk::streamread(stream, nparams);
            n += idk::streamread(stream, subject_id);
            n += idk::streamread(stream, target_id);
            n += idk::streamread(stream, dependency);
            n += idk::streamread(stream, retvalue);
            n += idk::streamread(stream, filepath);
            return n;
        };

        void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            this->obj_id = obj_id;
            L = idk::LuaManager::newState();
        };
    
        void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
        {
            lua_close(L);
        };

        void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
        {
            auto &src_cmp = api.getECS().getComponent<idk::ScriptCmp>(src_obj);
            auto &dst_cmp = api.getECS().getComponent<idk::ScriptCmp>(dst_obj);

            dst_cmp.obj_id      = src_cmp.obj_id;
            dst_cmp.L           = idk::LuaManager::newState();
            dst_cmp.nparams     = src_cmp.nparams;
            dst_cmp.subject_id  = src_cmp.subject_id;
            dst_cmp.target_id   = src_cmp.target_id;
            dst_cmp.dependency  = src_cmp.dependency;
            dst_cmp.retvalue    = src_cmp.retvalue;
            dst_cmp.filepath    = src_cmp.filepath;
        };

    };

    int ScriptCmp_getNumParams( idk::ScriptCmp & );


    struct ModelCmp
    {
        int         obj_id      = -1;
        int         model_id    = -1;
        bool        visible     = true;
        bool        shadowcast  = true;
        bool        viewspace   = false;
        std::string filepath    = "";

        bool        shader_enabled = false;
        int         render_queue   = -1;
        std::string shader_name    = "";


        size_t serialize( std::ofstream &stream ) const
        {
            size_t n = 0;
            n += idk::streamwrite(stream, obj_id);
            n += idk::streamwrite(stream, model_id);
            n += idk::streamwrite(stream, visible);
            n += idk::streamwrite(stream, shadowcast);
            n += idk::streamwrite(stream, viewspace);
            n += idk::streamwrite(stream, filepath);
            n += idk::streamwrite(stream, shader_enabled);
            n += idk::streamwrite(stream, render_queue);
            n += idk::streamwrite(stream, shader_name);
            return n;
        };

        size_t deserialize( std::ifstream &stream )
        {
            size_t n = 0;
            n += idk::streamread(stream, obj_id);
            n += idk::streamread(stream, model_id);
            n += idk::streamread(stream, visible);
            n += idk::streamread(stream, shadowcast);
            n += idk::streamread(stream, viewspace);
            n += idk::streamread(stream, filepath);
            n += idk::streamread(stream, shader_enabled);
            n += idk::streamread(stream, render_queue);
            n += idk::streamread(stream, shader_name);
            return n;
        };

        void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            this->obj_id = obj_id;
        };

        void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
        {

        };

        void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
        {
            ModelCmp &src = api.getECS().getComponent<ModelCmp>(src_obj);
            
            model_id    = src.model_id;
            visible     = src.visible;
            shadowcast  = src.shadowcast;
            viewspace   = src.viewspace;
            filepath    = src.filepath;
        };

    };


    struct CameraCmp
    {
        int   obj_id   = -1;
        int   cam_id   = -1;
        float gamma    = 2.2f;
        float exposure = 1.0f;
        float bloom    = 0.0f;
        float fov      = 90.0f;

        glm::vec2 chromatic_r;
        glm::vec2 chromatic_g;
        glm::vec2 chromatic_b;
        glm::vec4 chromatic_strength;
    
        size_t serialize( std::ofstream &stream ) const
        {
            size_t n = 0;
            n += idk::streamwrite(stream, obj_id);
            n += idk::streamwrite(stream, cam_id);
            n += idk::streamwrite(stream, gamma);
            n += idk::streamwrite(stream, exposure);
            n += idk::streamwrite(stream, bloom);
            n += idk::streamwrite(stream, fov);
            n += idk::streamwrite(stream, chromatic_r);
            n += idk::streamwrite(stream, chromatic_g);
            n += idk::streamwrite(stream, chromatic_b);
            n += idk::streamwrite(stream, chromatic_strength);
            return n;
        };

        size_t deserialize( std::ifstream &stream )
        {
            size_t n = 0;
            n += idk::streamread(stream, obj_id);
            n += idk::streamread(stream, cam_id);
            n += idk::streamread(stream, gamma);
            n += idk::streamread(stream, exposure);
            n += idk::streamread(stream, bloom);
            n += idk::streamread(stream, fov);
            n += idk::streamread(stream, chromatic_r);
            n += idk::streamread(stream, chromatic_g);
            n += idk::streamread(stream, chromatic_b);
            n += idk::streamread(stream, chromatic_strength);
            return n;
        };


        void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            this->obj_id = obj_id;
        };

        void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
        void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj ) {  };

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

        void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            this->obj_id = obj_id;
        };
    
        void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
        {
        };

        void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
        {

        };

    };


    struct AudioListenerCmp
    {
        #define IDK_STRUCT_MEMBERS(X) \
            X( int, obj_id, -1 ) \
    
        IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
        #undef IDK_STRUCT_MEMBERS

        void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            this->obj_id = obj_id;
        };

        void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
        void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj ) {  };

    };

};




