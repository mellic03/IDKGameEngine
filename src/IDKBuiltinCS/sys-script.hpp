#pragma once

#include <IDKECS/IDKECS.hpp>
#include <libidk/idk_cppscript.hpp>



namespace idk
{
    struct ScriptCmp;
    class  ScriptSys;
}




class idk::ScriptSys: public idk::ECS2::System
{
private:

public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

    static void     assignScript( int obj_id, const std::string &filepath );

};



struct idk::ScriptCmp
{
    int           obj_id   = -1;
    std::string   filepath = "";
    RuntimeScript script;

    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );

};


