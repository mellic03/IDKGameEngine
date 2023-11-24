#pragma once

#include "IDKmodel/IDKmodel.h"
#include "IDKcommon/IDKgl.hpp"


namespace idk { class ModelManager; };


class idk::ModelManager
{
private:

    Allocator<Material>     _materials;
    Allocator<Model>        _models;

    std::unordered_map<std::string, idk::Texture>   _textures;

    struct DefaultID { bool set=false; GLuint texture_ID; };
    GLuint m_default_texture_ID = 0;

    std::unordered_map<std::string, DefaultID>      _texture_IDs;
    std::unordered_map<std::string, int>            _material_IDs;

    void                _load_mtl( std::string );
    idk::Model          _load_obj( std::string );

    GLuint              get_texture_id( const std::string &key );

public:

                        ModelManager();

    int                 loadOBJ( std::string raw_obj, std::string raw_mtl );
    int                 loadOBJ( std::string rootpath, std::string obj, std::string mtl );
    
                        /** Load only vertices from .obj file */
    void                loadVertices( std::string filepath, std::vector<idk::Vertex> &vertices );

    void                loadTexture  ( std::string filepath, bool srgb );
    void                loadTextures ( std::string rootpath, bool srgb );


    Model &             getModel( int id )  { return _models.get(id); };
    
    Allocator<Material> &getMaterials() { return _materials; };
    auto                &getTextures()  { return _textures;  };

};

