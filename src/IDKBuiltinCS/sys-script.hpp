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
    std::map<std::string, RuntimeScript*> m_scripts;

public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

    // void reserve      ( int obj_id, int count );
    void reloadScript ( const std::string &path );
    int  attachScript ( int obj_id, const std::string &path, void *data=nullptr, bool autoexec=true );
    void removeScript ( int obj_id, const std::string &name );
    void attachData   ( int obj_id, int idx, void *data );

    void executeScript( int obj_id, const std::string &name, void *data=nullptr );

    RuntimeScript *getScript( const std::string &name );

};



struct idk::ScriptCmp
{
    int  obj_id = -1;

    std::vector<void*>         data;
    std::vector<std::string>   scripts;
    std::vector<int>           timers;
    std::vector<bool>          autoexec;

    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj );

};


