#include "sys-rendersetting.hpp"
#include "sys-transform.hpp"
#include "sys-lightsource.hpp"

#include <libidk/idk_geometry.hpp>
#include <libidk/idk_log2.hpp>


static idk::EngineAPI *api_ptr;


static idk::RenderSettingCmp &getCmp()
{
    auto &ecs = api_ptr->getECS();
    return *(ecs.getComponentArray<idk::RenderSettingCmp>().begin());
}



void
idk::RenderSettingSys::init( idk::EngineAPI &api )
{
    LOG_INFO("idk::RenderSettingSys::init", "");
    api_ptr = &api;
}



void
idk::RenderSettingSys::update( idk::EngineAPI &api )
{
    auto  &ecs  = api.getECS();
    auto  &ren  = api.getRenderer();
    float dtime = api.dtime();

    static std::string current_filepath = "assets/cubemaps/skybox8/";

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

        // ren.applyRenderSettings(cmp.settings, false);
    }

}



void
idk::RenderSettingSys::shutdown( idk::EngineAPI &api )
{
    LOG_INFO("idk::RenderSettingSys::shutdown", "");
}




size_t
idk::RenderSettingCmp::serialize( std::ofstream &stream ) const
{
    auto config = api_ptr->getRenderer().getRenderSettings();

    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, filepath);
    n += idk::streamwrite(stream, config);

    return n;
};



size_t
idk::RenderSettingCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;

    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, filepath);
    n += idk::streamread(stream, settings);

    api_ptr->getRenderer().applyRenderSettings(settings);

    // this->skybox = api_ptr->getRenderer().loadSkybox(filepath);
    // api_ptr->getRenderer().useSkybox(this->skybox);

    return n;
};


void
idk::RenderSettingCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    ecs.giveComponent<DirlightCmp>(obj_id);
    auto &cmp = ecs.getComponent<TransformCmp>(obj_id);
    // cmp.transform.rotation = glm::quat(glm::normalize(glm::vec3(-1.0, -0.5, +1.0)));
    cmp.transform.rotation = -glm::quat(0.77, -0.45, -0.37, -0.21);

    // this->obj_id = obj_id;
};


void
idk::RenderSettingCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    // this->obj_id = -1;
};


void
idk::RenderSettingCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
{
    // auto &src = ecs.getComponent<RenderSettingCmp>(src_obj);
    // auto &dst = ecs.getComponent<RenderSettingCmp>(dst_obj);
    // dst.visualise = src.visualise;
};

