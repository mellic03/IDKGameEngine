#pragma once

#include <IDKECS/IDKECS.hpp>


namespace idk
{
    struct TriggerCmp;
    class  TriggerSys;
}



class idk::TriggerSys: public idk::ECS::System
{
private:

public:
    using trigger_type  = std::function<bool(idk::EngineAPI&, int)>;
    using callback_type = std::function<void(idk::EngineAPI&, int)>;


    virtual void init   ( idk::EngineAPI & ) final;
    virtual void update ( idk::EngineAPI & ) final;

    static void registerTrigger ( const std::string &name, trigger_type );
    static void registerResponse( const std::string &name, callback_type );

};



struct idk::TriggerCmp
{
    int obj_id = -1;

    std::string trigger  = "";
    std::string response = "";

    size_t      serialize            ( std::ofstream &stream ) const;
    size_t      deserialize          ( std::ifstream &stream );
    static void onObjectAssignment   ( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy         ( int src_obj, int dst_obj );
};

