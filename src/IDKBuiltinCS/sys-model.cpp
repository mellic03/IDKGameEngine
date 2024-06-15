#include "sys-model.hpp"

#include "sys-transform.hpp"
static idk::EngineAPI *api_ptr;


void
idk::ModelSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;
    auto &ren = api.getRenderer();


    m_heightmap_program = ren.createProgram(
        "ModelSys-terrain", "assets/shaders/", "gpass-terrain.vs", "gpass-terrain.fs"
    );

    m_heightmap_RQ = ren.createRenderQueue("ModelSys-terrain");

    for (auto &cmp: ECS2::getComponentArray<idk::ModelCmp>())
    {
        if (cmp.filepath[0] != '\0')
        {
            cmp.model_id = ren.loadModel(cmp.filepath);
        }
    }
}



void
idk::ModelSys::update( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    

    for (idk::ModelCmp &cmp: idk::ECS2::getComponentArray<idk::ModelCmp>())
    {
        if (cmp.model_id == -1 || cmp.visible == false)
        {
            continue;
        }

        glm::mat4 transform = TransformSys::getModelMatrix(cmp.obj_id);

        ren.drawModel(cmp.model_id, transform);

        if (cmp.shadowcast)
        {
            ren.drawShadowCaster(cmp.model_id, transform);
        }
    }


    if (m_heightmap_RQ == -1)
    {
        m_heightmap_RQ = ren.createRenderQueue("ModelSys-terrain");
    }

    for (auto &cmp: ECS2::getComponentArray<StaticHeightmapCmp>())
    {
        if (cmp.model == -1)
        {
            static const idk::glTextureConfig config = {
                .internalformat = GL_RGBA8,
                .format         = GL_RGBA,
                .minfilter      = GL_LINEAR,
                .magfilter      = GL_LINEAR,
                .wrap_s         = GL_CLAMP_TO_BORDER,
                .wrap_t         = GL_CLAMP_TO_BORDER,
                .datatype       = GL_UNSIGNED_BYTE,
                .genmipmap      = GL_FALSE
            };

            uint32_t texture = idk::gltools::loadTexture("assets/heightmaps/terrain.png", config);
            cmp.model = api_ptr->getRenderer().loadModel("assets/models/plane.idkvi");
            ren.modelAllocator().addUserMaterials(cmp.model, {texture});
        }

        glm::mat4 M = TransformSys::getModelMatrix(cmp.obj_id);
        std::cout << "Model: " << cmp.model << "\n";
        ren.drawModelRQ(m_heightmap_RQ, cmp.model, M);
    }

}


void
idk::ModelSys::assignModel( int obj_id, const std::string &filepath )
{
    auto &cmp = idk::ECS2::getComponent<idk::ModelCmp>(obj_id);

    int model_id = api_ptr->getRenderer().loadModel(filepath);

    cmp.obj_id   = obj_id;
    cmp.model_id = model_id;
    cmp.filepath = filepath;
}


void
idk::ModelSys::assignShader_gpass( int obj_id, const std::string &shader_name )
{
    auto &cmp  = idk::ECS2::getComponent<idk::ModelCmp>(obj_id);
    cmp.shader_name = shader_name;
}



size_t
idk::StaticHeightmapCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, textures);
    return n;
};


size_t
idk::StaticHeightmapCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, textures);

    return n;
};


void
idk::StaticHeightmapCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    auto &cmp = idk::ECS2::getComponent<StaticHeightmapCmp>(obj_id);
    // cmp.model = api_ptr->getRenderer().loadModel(cmp.filepath);
};


void
idk::StaticHeightmapCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

};


void
idk::StaticHeightmapCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<StaticHeightmapCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<StaticHeightmapCmp>(dst_obj);

};



