#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>


namespace idk
{
    class  RenderSettingSys;
    struct RenderSettingCmp;
}



class idk::RenderSettingSys: public idk::ecs::System
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

    size_t  serialize            ( std::ofstream &stream ) const;
    size_t  deserialize          ( std::ifstream &stream );
    void    onObjectAssignment   ( idk::EngineAPI &api, int obj_id );
    void    onObjectDeassignment ( idk::EngineAPI &api, int obj_id );
    void    onObjectCopy         ( idk::EngineAPI &api, int src_obj, int dst_obj );
};

