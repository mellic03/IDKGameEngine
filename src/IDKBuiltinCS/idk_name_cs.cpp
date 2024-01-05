#include "idk_name_cs.hpp"


void
idk::Name_CS::init( idk::EngineAPI &api )
{

}


void
idk::Name_CS::setName( int obj_id, const std::string &name )
{
    int key = m_keys[obj_id];
    m_names.get(key) = name; 
}


std::string &
idk::Name_CS::getName( int obj_id )
{
    int key = m_keys[obj_id];

    if (key == -1)
    {
        static std::string temp = "Object does not exist";
        return temp;
    }

    return m_names.get(key);
}


void
idk::Name_CS::onObjectAssignment( int obj_id, idk::Engine &engine )
{
    m_keys[obj_id] = m_names.create("Object");
}


void
idk::Name_CS::onObjectCreation( int obj_id, idk::Engine &engine )
{
    engine.giveComponent(obj_id, this->ID());
}


void
idk::Name_CS::onObjectDeletion( int obj_id, idk::Engine &engine )
{
    int &key = m_keys[obj_id];
    m_names.destroy(key);
    key = -1;
}


void
idk::Name_CS::onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{
    // engine.giveComponent(dest_obj_id, this->m_id);
    // setName(dest_obj_id, getName(src_obj_id));
}