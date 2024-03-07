#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>



namespace idk
{
    struct PlayerControllerCmp;
    class  PlayerSys;
}



class idk::PlayerSys: public idk::ecs::System
{
private:

    static void     _keyInput( idk::EngineAPI &api, int player );
    static void     _mouseInput( idk::EngineAPI &api, int player );


public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

};


struct idk::PlayerControllerCmp
{
    int obj_id = -1;

    size_t  serialize( std::ofstream &stream ) const;
    size_t  deserialize( std::ifstream &stream );
    void    onObjectAssignment( idk::EngineAPI &api, int obj_id );
    void    onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    void    onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj );
};


