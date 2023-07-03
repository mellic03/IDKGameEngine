#pragma once

#include "idk_model.h"
#include "idk_filetypes.h"

namespace idk { class ModelManager; };


class idk::ModelManager
{
private:

    Allocator<Material>     _materials;
    Allocator<Model>        _models;

    std::unordered_map<std::string, GLuint>     _texture_gl_IDs;
    std::unordered_map<std::string, int>        _material_IDs;

    void                    _load_mtl( std::string );
    idk::Model              _load_obj( std::string );


public:

    int         loadOBJ( std::string raw_obj, std::string raw_mtl );
    int         loadOBJ( std::string rootpath, std::string obj, std::string mtl );

    void        addTexture( std::string name, GLuint gl_id, int w, int h, uint8_t* &data );


    Model &     getModel( int id )  { return _models.get(id); };
    
    Allocator<Material> &getMaterials() { return _materials; };

};

