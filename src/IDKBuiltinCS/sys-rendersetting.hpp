#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>
#include <IDKGraphics/idk_render_settings.hpp>


namespace idk
{
    class  RenderSettingSys;
    struct RenderSettingCmp;
}



class idk::RenderSettingSys: public idk::ECS2::System
{
private:
    inline static int m_obj = -1;

public:
    virtual void  init     ( idk::EngineAPI& ) final;
    virtual void  update   ( idk::EngineAPI& ) final;
    virtual void  shutdown ( idk::EngineAPI& ) final;

};



struct idk::RenderSettingCmp
{
    int obj_id = -1;
    int skybox = -1;
    std::string filepath = "";
    idk::RenderSettings settings;

    size_t serialize   ( std::ofstream &stream ) const;
    size_t deserialize ( std::ifstream &stream );
    static void onObjectAssignment   ( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy         ( int src_obj, int dst_obj );
};

