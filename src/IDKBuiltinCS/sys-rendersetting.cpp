#include "sys-rendersetting.hpp"
#include "sys-transform.hpp"

#include <libidk/idk_geometry.hpp>
#include <libidk/idk_log.hpp>


static idk::EngineAPI *api_ptr;
static idk::ecs::ECS &getECS() { return api_ptr->getECS(); }

static idk::RenderSettingCmp &getCmp()
{
    return *(getECS().getComponentArray<idk::RenderSettingCmp>().begin());
}



void
idk::RenderSettingSys::init( idk::EngineAPI &api )
{
    LOG_INFO() << "idk::RenderSettingSys::init";

    api_ptr = &api;
    auto &ecs = api.getECS();

}



void
idk::RenderSettingSys::update( idk::EngineAPI &api )
{
    auto  &ren  = api.getRenderer();
    auto  &ecs  = api.getECS();
    float dtime = api.getEngine().deltaTime();

    static std::string current_filepath = "assets/cubemaps/skybox5/";

    if (m_obj == -1)
    {
        m_obj = ecs.createGameObject("RenderSetting");
        ecs.giveComponent<idk::RenderSettingCmp>(m_obj);
    }

    for (auto &cmp: ecs.getComponentArray<idk::RenderSettingCmp>())
    {
        if (cmp.filepath != "" && cmp.filepath != current_filepath)
        {
            cmp.skybox = ren.loadSkybox(cmp.filepath);
            ren.useSkybox(cmp.skybox);
            current_filepath = cmp.filepath;
        }
    }

}



void
idk::RenderSettingSys::shutdown( idk::EngineAPI &api )
{
    LOG_INFO() << "idk::RenderSettingSys::shutdown";
}




size_t
idk::RenderSettingCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, filepath);
    return n;
};


size_t
idk::RenderSettingCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, filepath);
    return n;
};


void
idk::RenderSettingCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    this->obj_id = obj_id;
};


void
idk::RenderSettingCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    this->obj_id = -1;
};


void
idk::RenderSettingCmp::onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
{
    // auto &src = api.getECS().getComponent<RenderSettingCmp>(src_obj);
    // auto &dst = api.getECS().getComponent<RenderSettingCmp>(dst_obj);
    // dst.visualise = src.visualise;
};

