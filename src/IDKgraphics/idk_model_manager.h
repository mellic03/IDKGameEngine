#pragma once

#include "IDKmodel/IDKmodel.h"
#include "IDKglInterface/IDKglInterface.h"


namespace idk { class ModelManager; };


class idk::ModelManager
{
private:

    Allocator<Material>     _materials;
    Allocator<Model>        _models;

    std::unordered_map<std::string, idk::Texture>   _textures;
    std::unordered_map<std::string, GLuint>         _texture_linr_IDs;
    std::unordered_map<std::string, GLuint>         _texture_srgb_IDs;
    std::unordered_map<std::string, int>            _material_IDs;

    void                    _load_mtl( std::string );
    idk::Model              _load_obj( std::string );

public:

    int                 loadOBJ( std::string raw_obj, std::string raw_mtl );
    int                 loadOBJ( std::string rootpath, std::string obj, std::string mtl );
    
                        /** Load only vertices from .obj file */
    void                loadVertices( std::string filepath, std::vector<idk::Vertex> &vertices );

    void                loadTexture( std::string filepath );
    void                loadTextures( std::string rootpath );

    void                loadTEX( std::string filepath );
    void                loadTEXs( std::string rootpath );

    Model &             getModel( int id )  { return _models.get(id); };
    
    Allocator<Material> &getMaterials() { return _materials; };
    auto                &getTextures()  { return _textures;  };

};

