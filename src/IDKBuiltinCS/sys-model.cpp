#include "sys-model.hpp"
#include "sys-transform.hpp"

#include <IDKGraphics/terrain/desc.hpp>



static idk::EngineAPI *api_ptr;


void
idk::ModelSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;
    auto &ecs = getECS();
    auto &ren = api.getRenderer();

    // ren.createProgram(
    //     "ModelSys-terrain", "assets/shaders/", "terrain-gpass.vs", "terrain-gpass.fs"
    // );

    // ren.createProgram(
    //     "ModelSys-terrain-shadow", "assets/shaders/", "terrain-shadow.vs", "terrain-shadow.fs"
    // );

    ren.createProgram(
        "ModelSys-gpass-alpha-cutoff", "IDKGE/shaders/deferred/", "gpass-alpha-cutoff.vs", "gpass-alpha-cutoff.fs"
    );

    // m_heightmap_RQ    = ren.createRenderQueue("ModelSys-terrain");
    // m_shadow_RQ       = ren.createShadowCasterQueue("ModelSys-terrain-shadow");
    m_alpha_cutoff_RQ = ren.createRenderQueue("ModelSys-gpass-alpha-cutoff", {false});

    for (auto &cmp: ecs.getComponentArray<idk::ModelCmp>())
    {
        // if (cmp.filepath[0] != '\0')
        // {
        //     cmp.model_id = ren.loadModel(cmp.filepath);
        // }
    }
}



void
idk::ModelSys::update( idk::EngineAPI &api )
{
    api_ptr = &api;
    auto &ecs  = getECS();
    auto &tsys = ecs.getSystem<TransformSys>();
    auto &ren  = api.getRenderer();
    
    for (auto &cmp: ecs.getComponentArray<TerrainCmp>())
    {
        // if (cmp.terrain_id == -1)
        // {
        //     TerrainCmp::onObjectAssignment(api, cmp.obj_id);
        // }

        glm::mat4 M = tsys.getModelMatrix(cmp.obj_id);
        // glm::mat4 M = glm::translate(glm::mat4(1), glm::vec3(0, -700, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(2048));
        TerrainRenderer::setTerrainTransform(M);
    }


    for (auto &cmp: ecs.getComponentArray<ModelCmp>())
    {
        if (cmp.model_id == -1 || cmp.visible == false)
        {
            continue;
        }

        const glm::mat4 &Tcurr = tsys.getModelMatrix(cmp.obj_id);
        const glm::mat4 &Tprev = tsys.getTransformCmp(cmp.obj_id).prev_model;

        if (cmp.custom_RQ != -1)
        {
            ren.drawModelRQ(cmp.custom_RQ, cmp.model_id, Tcurr, Tprev);
        }

        else if (cmp.alpha_cutoff)
        {
            ren.drawModelRQ(m_alpha_cutoff_RQ, cmp.model_id, Tcurr, Tprev);
        }

        else if (cmp.viewspace)
        {
            ren.drawModelViewspace(cmp.model_id, Tcurr, Tprev);
        }

        else
        {
            ren.drawModel(cmp.model_id, Tcurr, Tprev);
        }

        // if (cmp.environment)
        // {
        //     ren.drawEnvironmental(cmp.model_id, transform);
        // }

        if (cmp.shadowcast)
        {
            ren.drawShadowCaster(cmp.model_id, Tcurr);
        }

    }


    static const idk::glTextureConfig config = {
        .internalformat = GL_RGBA8,
        .format         = GL_RGBA,
        .minfilter      = GL_LINEAR_MIPMAP_LINEAR,
        .magfilter      = GL_LINEAR,
        .wrap_s         = GL_REPEAT,
        .wrap_t         = GL_REPEAT,
        .datatype       = GL_UNSIGNED_BYTE,
        .genmipmap      = GL_TRUE
    };

    // if (m_heightmap_RQ == -1)
    // {
    //     m_heightmap_RQ = ren.createRenderQueue("ModelSys-terrain");
    // }

    // if (m_shadow_RQ == -1)
    // {
    //     m_shadow_RQ = ren.createShadowCasterQueue("ModelSys-terrain-shadow");
    // }


    // for (auto &cmp: ecs.getComponentArray<StaticHeightmapCmp>())
    // {
    //     if (cmp.textures.empty() == false && cmp.textures[0] != "")
    //     {
    //         if (cmp.textures.back() == "")
    //         {
    //             cmp.textures.pop_back();
    //         }
        
    //         if (cmp.textures.empty() == false)
    //         {
    //             ren.modelAllocator().addUserMaterials(cmp.model, cmp.textures, config);
    //         }
    //     }

    //     glm::mat4 M = TransformSys::getModelMatrix(cmp.obj_id);
    //     ren.drawModelRQ(m_heightmap_RQ, cmp.model, M);
    //     ren.drawShadowCasterRQ(m_shadow_RQ, cmp.model, M);
    // }

}


void
idk::ModelSys::assignModel( int obj_id, const char *filepath )
{
    auto &ecs = getECS();

    if (ecs.hasComponent<ModelCmp>(obj_id) == false)
    {
        ecs.giveComponent<ModelCmp>(obj_id);
    }

    auto &cmp = ecs.getComponent<idk::ModelCmp>(obj_id);
    int model_id = api_ptr->getRenderer().loadModel(filepath);

    cmp.obj_id   = obj_id;
    cmp.model_id = model_id;
    cmp.filepath = filepath;
}


void
idk::ModelSys::assignModelLOD( int obj_id, int level, const char *filepath )
{
    auto &ecs = getECS();
    auto &cmp = ecs.getComponent<idk::ModelCmp>(obj_id);
    api_ptr->getRenderer().loadModelLOD(cmp.model_id, level, filepath);
}

void
idk::ModelSys::assignCustomRQ( int obj_id, int RQ )
{
    auto &ecs = getECS();
    auto &cmp = ecs.getComponent<idk::ModelCmp>(obj_id);
    cmp.custom_RQ = RQ;
}


void
idk::ModelSys::assignShader_gpass( int obj_id, const char *shader_name )
{
    auto &ecs = getECS();
    auto &cmp  = ecs.getComponent<idk::ModelCmp>(obj_id);
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

    static const idk::glTextureConfig config = {
        .internalformat = GL_RGBA8,
        .format         = GL_RGBA,
        .minfilter      = GL_LINEAR_MIPMAP_LINEAR,
        .magfilter      = GL_LINEAR,
        .wrap_s         = GL_REPEAT,
        .wrap_t         = GL_REPEAT,
        .datatype       = GL_UNSIGNED_BYTE,
        .genmipmap      = GL_TRUE
    };

    gltools::loadTexture("assets/heightmaps/sand-dunes.jpg", config, &(this->heightmap));
    this->model = api_ptr->getRenderer().loadModel("assets/models/unit-plane.idkvi");

    return n;
};


void
idk::StaticHeightmapCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<StaticHeightmapCmp>(obj_id);

    static const idk::glTextureConfig config = {
        .internalformat = GL_RGBA8,
        .format         = GL_RGBA,
        .minfilter      = GL_LINEAR_MIPMAP_LINEAR,
        .magfilter      = GL_LINEAR,
        .wrap_s         = GL_REPEAT,
        .wrap_t         = GL_REPEAT,
        .datatype       = GL_UNSIGNED_BYTE,
        .genmipmap      = GL_TRUE
    };

    gltools::loadTexture("assets/heightmaps/sand-dunes.jpg", config, &(cmp.heightmap));
    cmp.model = api.getRenderer().loadModel("assets/models/unit-plane.idkvi");

};


void
idk::StaticHeightmapCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{

};


void
idk::StaticHeightmapCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
{
    auto &src = ecs.getComponent<StaticHeightmapCmp>(src_obj);
    auto &dst = ecs.getComponent<StaticHeightmapCmp>(dst_obj);

};









size_t
idk::ModelCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, model_id);
    n += idk::streamwrite(stream, visible);
    n += idk::streamwrite(stream, shadowcast);
    n += idk::streamwrite(stream, environment);
    n += idk::streamwrite(stream, alpha_cutoff);
    n += idk::streamwrite(stream, viewspace);
    n += idk::streamwrite(stream, filepath);
    n += idk::streamwrite(stream, shader_enabled);
    n += idk::streamwrite(stream, render_queue);
    n += idk::streamwrite(stream, shader_name);
    return n;
}


size_t
idk::ModelCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, model_id);
    n += idk::streamread(stream, visible);
    n += idk::streamread(stream, shadowcast);
    n += idk::streamread(stream, environment);
    n += idk::streamread(stream, alpha_cutoff);
    n += idk::streamread(stream, viewspace);
    n += idk::streamread(stream, filepath);
    n += idk::streamread(stream, shader_enabled);
    n += idk::streamread(stream, render_queue);
    n += idk::streamread(stream, shader_name);

    model_id = api_ptr->getRenderer().loadModel(filepath);

    return n;
}


void
idk::ModelCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    ModelCmp &cmp = ecs.getComponent<ModelCmp>(obj_id);

    if (cmp.model_id == -1 && cmp.filepath != "")
    {
        cmp.model_id = api.getRenderer().loadModel(cmp.filepath);
    }
}


void
idk::ModelCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{

}


void
idk::ModelCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
{
    ModelCmp &src = ecs.getComponent<ModelCmp>(src_obj);
    ModelCmp &dst = ecs.getComponent<ModelCmp>(dst_obj);
    
    dst.model_id    = src.model_id;
    dst.visible     = src.visible;
    dst.shadowcast  = src.shadowcast;
    dst.viewspace   = src.viewspace;
    dst.filepath    = src.filepath;
}












size_t
idk::TerrainCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);

    auto &desc = idk::TerrainRenderer::getTerrainDesc();
    n += idk::streamwrite(stream, desc);

    return n;
}


size_t
idk::TerrainCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);

    auto &desc = idk::TerrainRenderer::getTerrainDesc();
    n += idk::streamread(stream, desc);

    idk::TerrainRenderer::generateTerrain();
    idk::TerrainRenderer::generateGrass();

    return n;
}


void
idk::TerrainCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    // TerrainCmp &cmp = ecs.getComponent<TerrainCmp>(obj_id);

    // cmp.desc = {
    //     .height = {
    //         "IDKGE/resources/terrain/heightmap.jpg",
    //         "",
    //         "",
    //         ""
    //     },

    //     .albedo = {
    //         "assets/terrain-textures/Ground037_1K-JPG_Color.jpg",
    //         "assets/terrain-textures/rocks_ground_06_diff_1k.jpg",
    //         "assets/terrain-textures/Ground037_1K-JPG_AmbientOcclusion.jpg",
    //         ""
    //     },

    //     .normal = {
    //         "assets/terrain-textures/Ground037_1K-JPG_NormalGL.jpg",
    //         "assets/terrain-textures/rocks_ground_06_nor_gl_1k.jpg",
    //         "assets/terrain-textures/rocks_ground_06_nor_gl_1k.jpg",
    //         ""
    //     },

    //     .ao_r_m = {
    //         "assets/terrain-textures/Ground037_1K-JPG_Roughness.jpg",
    //         "assets/terrain-textures/rocks_ground_06_arm_1k.jpg",
    //         "assets/terrain-textures/rocks_ground_06_arm_1k.jpg",
    //         ""
    //     },

    // };

    // cmp.terrain_id = TerrainRenderer::createTerrain(cmp.desc);
}


void
idk::TerrainCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{

}


void
idk::TerrainCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
{

}



