#pragma once

#include "model/IDKmodel.hpp"
#include "libidk/IDKgl.hpp"


namespace idk { class ModelManager; };


class idk::ModelManager
{
private:

    Allocator<Material>         m_materials;
    Allocator<Model>            m_models;

    struct DefaultID { bool set=false; GLuint texture_ID; };
    GLuint m_default_albedo    = 0;
    GLuint m_default_metallic  = 0;
    GLuint m_default_roughness = 0;
    GLuint m_default_ao        = 0;
    GLuint m_default_normal    = 0;
    GLuint m_default_height    = 0;
    GLuint m_default_rm        = 0;

    std::unordered_map<std::string, DefaultID>      _texture_IDs;
    std::unordered_map<std::string, int>            _material_IDs;

    GLuint              get_texture_id( const std::string &key, bool srgb );

    void                model_to_gpu( idk::Model &model );
    int                 new_material();

public:

    void                init();

    int                 loadModel( const std::string &root, const std::string &name );

                        /** Load only vertices from .obj file */
    void                loadVertices( std::string filepath, std::vector<idk::Vertex> &vertices );

    void                loadTexture  ( std::string filepath, bool srgb, GLint minfilter, GLint magfilter );
    void                loadTextures ( std::string rootpath, bool srgb, GLint minfilter, GLint magfilter );

    Model &             getModel         ( int id )  { return m_models.get(id); };
    
    Allocator<Material> &getMaterials() { return m_materials; };

};

