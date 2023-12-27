#pragma once

#include <libidk/IDKglm.hpp>
#include <idk_allocator.hpp>

#include "model/IDKmodel.hpp"
#include "animation/IDKanimation.hpp"


namespace idk { class ModelSystem; };


class idk::ModelSystem
{
private:

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

    GLuint              get_texture_id( const std::string &key, const glTextureConfig &config );

    void                model_to_gpu( idk::Model &model );
    void                instanced_to_gpu( idk::Model &, const std::vector<glm::mat4> & );

    int                 new_material();


public:

    void                init();

    void                loadTexture( std::string filepath, const glTextureConfig &config );
    int                 loadModel( const std::string &root, const std::string &name );

    Model &             getModel      ( int id       ) { return m_models.get(id);                   };
    Animator &          getAnimator   ( int id       ) { return m_animators.get(id);                };
    int                 getAnimatorID ( int model_id ) { return m_models.get(model_id).animator_id; };

    int                 createAnimator();
    int                 copyAnimator( int model_id );

    int                 createInstancedModel( int model_id, const std::vector<glm::mat4> & );

    int                 createChunkedModel( int model_id,
                                            const std::vector<glm::vec4> &positions,
                                            const std::vector<idk::OBB>  &OBBs,
                                            const std::vector<std::vector<glm::mat4>> & );
                                        


    Allocator<Material> &getMaterials() { return m_materials; };
    idk::Material       &getMaterial( int id ) { return m_materials.get(id); };

};

