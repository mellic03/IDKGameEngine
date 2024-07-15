#include "sys-rendersetting.hpp"
#include "sys-transform.hpp"

#include <libidk/idk_geometry.hpp>
#include <libidk/idk_log.hpp>


static idk::EngineAPI *api_ptr;


static idk::RenderSettingCmp &getCmp()
{
    return *(idk::ECS2::getComponentArray<idk::RenderSettingCmp>().begin());
}



void
idk::RenderSettingSys::init( idk::EngineAPI &api )
{
    LOG_INFO() << "idk::RenderSettingSys::init";
    api_ptr = &api;
}



void
idk::RenderSettingSys::update( idk::EngineAPI &api )
{
    auto  &ren  = api.getRenderer();
    float dtime = api.getEngine().deltaTime();

    static std::string current_filepath = "assets/cubemaps/skybox5/";

    if (m_obj == -1)
    {
        m_obj = idk::ECS2::createGameObject("RenderSetting");
        idk::ECS2::giveComponent<idk::RenderSettingCmp>(m_obj);
    }

    for (auto &cmp: idk::ECS2::getComponentArray<idk::RenderSettingCmp>())
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

    // this->skybox = api_ptr->getRenderer().loadSkybox(filepath);
    // api_ptr->getRenderer().useSkybox(this->skybox);

    return n;
};


void
idk::RenderSettingCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = obj_id;
};


void
idk::RenderSettingCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = -1;
};


void
idk::RenderSettingCmp::onObjectCopy( int src_obj, int dst_obj )
{
    // auto &src = idk::ECS2::getComponent<RenderSettingCmp>(src_obj);
    // auto &dst = idk::ECS2::getComponent<RenderSettingCmp>(dst_obj);
    // dst.visualise = src.visualise;
};

