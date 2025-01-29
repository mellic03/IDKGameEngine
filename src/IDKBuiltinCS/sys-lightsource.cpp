#include "sys-lightsource.hpp"
#include "sys-transform.hpp"

static idk::EngineAPI *api_ptr;



void
idk::LightSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;

}


void
idk::LightSys::update( idk::EngineAPI &api )
{
    auto &ecs  = api.getECS();
    auto &tsys = ecs.getSystem<TransformSys>();
    auto &ren  = api.getRenderer();

    for (auto &cmp: ecs.getComponentArray<DirlightCmp>())
    {
        glm::mat3 R = tsys.getModelMatrix(cmp.obj_id);
        cmp.light.direction = glm::vec4(R*glm::vec3(0, 0, -1), 0.0f);
        ren.getDirlight(cmp.light_id) = cmp.light;
    }


    for (auto &cmp: ecs.getComponentArray<PointlightCmp>())
    {
        cmp.light.position = glm::vec4(tsys.getPositionWorldspace(cmp.obj_id), 1.0f);
        ren.getPointlight(cmp.light_id) = cmp.light;
    }


    for (auto &cmp: ecs.getComponentArray<SpotlightCmp>())
    {
        cmp.light.direction = glm::vec4(tsys.getFront(cmp.obj_id), 0.0f);
        cmp.light.position  = glm::vec4(tsys.getPositionWorldspace(cmp.obj_id), 1.0f);

        ren.getSpotlight(cmp.light_id) = cmp.light;
    }

}






size_t
idk::DirlightCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, light);
    return n;
}


size_t
idk::DirlightCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, light);

    light_id = api_ptr->getRenderer().createDirlight();
    api_ptr->getRenderer().getDirlight(light_id) = this->light;

    return n;
}


void
idk::DirlightCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<DirlightCmp>(obj_id);
    cmp.light_id = api.getRenderer().createDirlight();
    cmp.light    = api.getRenderer().getDirlight(cmp.light_id);
}


void
idk::DirlightCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<DirlightCmp>(obj_id);
    api.getRenderer().destroyDirlight(cmp.light_id);
}


void
idk::DirlightCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
{
    
}








size_t
idk::PointlightCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, light);
    return n;
}


size_t
idk::PointlightCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, light);

    light_id = api_ptr->getRenderer().createPointlight();
    api_ptr->getRenderer().getPointlight(light_id) = this->light;

    return n;
}


void
idk::PointlightCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<PointlightCmp>(obj_id);
    cmp.light_id = api.getRenderer().createPointlight();
    cmp.light    = api.getRenderer().getPointlight(cmp.light_id);
}


void
idk::PointlightCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<PointlightCmp>(obj_id);
    api_ptr->getRenderer().destroyPointlight(cmp.light_id);
}


void
idk::PointlightCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
{
    
}








size_t
idk::SpotlightCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, light);
    return n;
}


size_t
idk::SpotlightCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, light);

    light_id = api_ptr->getRenderer().createSpotlight();
    api_ptr->getRenderer().getSpotlight(light_id) = this->light;

    return n;
}


void
idk::SpotlightCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<SpotlightCmp>(obj_id);
    cmp.light_id = api.getRenderer().createSpotlight();
    cmp.light    = api.getRenderer().getSpotlight(cmp.light_id);
}


void
idk::SpotlightCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<SpotlightCmp>(obj_id);
    api_ptr->getRenderer().destroySpotlight(cmp.light_id);
}


void
idk::SpotlightCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
{
    
}





