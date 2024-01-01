#pragma once

#include <libidk/idk_glm.hpp>
#include <libidk/idk_allocator.hpp>

#include "model/IDKmodel.hpp"
#include "animation/IDKanimation.hpp"


namespace idk { class ModelSystem; };


class IDK_VISIBLE idk::ModelSystem
{
private:

    static constexpr size_t             MAX_PLANE_LEVEL = 7;
    std::array<int, MAX_PLANE_LEVEL>    m_planes;
    int                                 m_plane_id = -1;

    idk::glSSBO                 m_texture_SSBO;
    idk::Buffer<GLuint64>       m_texture_handles;
    // std::vector<GLuint64>       m_texture_handles;

    Allocator<Model_Terrain>    m_terrain_models;

    Allocator<glTexture>                 m_textures;
    std::unordered_map<GLuint, int>      m_rtexture_IDs;

    Allocator<Material>         m_materials;
    Allocator<Model>            m_models;
    Allocator<Animator>         m_animators;

    GLuint                      m_default_albedo = 0;
    GLuint                      m_default_ao_r_m = 0;
    GLuint                      m_default_normal = 0;
    GLuint                      m_default_height = 0;

    glTextureConfig             m_default_albedo_config;
    glTextureConfig             m_default_lightmap_config;

    struct DefaultID { bool set=false; GLuint texture_ID; };
    std::unordered_map<std::string, DefaultID>      m_texture_IDs;
    std::unordered_map<std::string, int>            m_material_IDs;

    // GLuint              get_texture_id( const std::string &key, const glTextureConfig &config );

    void                model_to_gpu( idk::Model &model );
    void                instanced_to_gpu( idk::Model &, const std::vector<glm::mat4> & );


    void                _gen_terrain_chunks();

    bool                _texture_loaded( const std::string &filepath );
    int                 _texture_ID( const std::string &filepath );



public:

    void                init();

    GLuint              loadTexture ( const std::string &, const glTextureConfig & );
    GLuint              loadTexture ( const std::string &, bool is_lightmap );
    GLuint              loadTexture ( size_t, size_t, void *, const glTextureConfig & );

    GLuint              getTexture  ( const std::string &, const glTextureConfig & );
    GLuint              getTexture  ( const std::string & );

    idk::glTexture &    getidkTexture( GLuint gl_id );


    int                 loadMaterial( const std::string &root,
                                      const std::string &albedo = "",
                                      const std::string &normal = "",
                                      const std::string &ao_rough_metal = "" );


    /** Create an idk::Material from three image textures.
     *  Setting any parameter to -1 will load the default image texture/lightmap. 
    */
    int                 createMaterial( int albedo, int normal, int ao_rough_metal );

    int                 loadModel( const std::string &root, const std::string &name );


    /**
        Load a terrain heightmap for displacing a subdivided plane.
        The plane is loaded on initialization so no model needs to be provided.
        @return model id 
    */
    int                 loadTerrainHeightmap( const std::string &filepath );
    int                 loadTerrainHeightmap( GLuint texture_id );
    void                loadTerrainMaterials( int terrain_id, int a, int b );

    /** Query the terrain heightmap for the corresponding y coordinate for a given x, z
    */
    float               queryTerrainHeight( int terrain_id, const glm::mat4 &transform,
                                            float x, float z );


    Model &             getModel( int id ) { return m_models.get(id); };
    Model_Terrain &     getModel_Terrain( int id ) { return m_terrain_models.get(id); };

    int                 copyModel( int id );

    Animator &          getAnimator( int id ) { return m_animators.get(id); };
    int                 getAnimatorID( int model_id ) { return m_models.get(model_id).animator_id; };

    int                 createAnimator();
    int                 copyAnimator( int model_id );

    int                 createInstancedModel( int model_id, const std::vector<glm::mat4> & );

    int                 createChunkedModel( int model_id,
                                            const std::vector<glm::vec4> &positions,
                                            const std::vector<std::vector<glm::mat4>> & );
                                        


    Allocator<Material> &getMaterials() { return m_materials; };
    idk::Material       &getMaterial( int id ) { return m_materials.get(id); };

};

