#pragma once

#include "model/IDKmodel.hpp"
#include "libidk/IDKgl.hpp"


namespace idk { class ModelManager; };


class idk::ModelManager
{
private:

    Allocator<Material>     m_materials;
    Allocator<Model>        m_models;
    Allocator<glInstancedTransforms> m_instancedata;

    struct DefaultID { bool set=false; GLuint texture_ID; };
    GLuint m_default_metallic  = 0;
    GLuint m_default_roughness = 0;
    GLuint m_default_ao        = 0;
    GLuint m_default_normal    = 0;
    GLuint m_default_height    = 0;

    std::unordered_map<std::string, DefaultID>      _texture_IDs;
    std::unordered_map<std::string, int>            _material_IDs;

    void                _load_mtl( std::string );
    idk::Model          _load_obj( std::string );

    GLuint              get_texture_id( const std::string &key, bool srgb );

public:

    void                init();

    int                 loadOBJ( std::string raw_obj, std::string raw_mtl );
    int                 loadOBJ( std::string rootpath, std::string obj, std::string mtl );
    
                        /** Load only vertices from .obj file */
    void                loadVertices( std::string filepath, std::vector<idk::Vertex> &vertices );

    void                loadTexture  ( std::string filepath, bool srgb, GLint minfilter, GLint magfilter );
    void                loadTextures ( std::string rootpath, bool srgb, GLint minfilter, GLint magfilter );

    void                loadTexturePBR  ( std::string filepath );
    void                loadTexturesPBR ( std::string rootpath );


    Model &             getModel( int id )  { return m_models.get(id); };
    glInstancedTransforms &getInstanceData( int model_id ) { return m_instancedata.get(model_id); };
    
    Allocator<Material> &getMaterials() { return m_materials; };

};

