#pragma once

#include <IDKECS/IDKECS.hpp>
// #include <libidk/idk_wallocator.hpp>
#include <libidk/idk_cppscript.hpp>

#include <map>



namespace idk
{
    struct ScriptCmp;
    class  ScriptSys;
}




class idk::ScriptSys: public idk::ECS::System
{
private:
    inline static std::map<std::string, RuntimeScript*> m_scripts;

public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

    static void     reserve      ( int obj_id, int count );
    static void     reloadScript ( const std::string &path );
    static void     attachScript ( int obj_id, int idx, const std::string &path );
    static void     attachData   ( int obj_id, int idx, void *data );

};



struct idk::ScriptCmp
{
    int  obj_id = -1;

    std::vector<void*>         data;
    std::vector<std::string>   scripts;
    std::vector<int>           timers;

    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );

};


