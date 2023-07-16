#include "idk_filetools.h"
#include <stdlib.h>


std::ofstream &operator << (std::ofstream &stream, idk::__tex_file_t &tex)
{
    size_t strsize = strlen(tex.name) + 1;
    stream.write(reinterpret_cast<const char *>(&strsize), sizeof(size_t));
   
    stream.write(reinterpret_cast<const char *>(tex.name), sizeof(char)*strsize);    
    stream.write(reinterpret_cast<const char *>(&tex.w), sizeof(size_t));
    stream.write(reinterpret_cast<const char *>(&tex.h), sizeof(size_t));
    stream.write(reinterpret_cast<const char *>(tex.data),  tex.w*tex.h*sizeof(uint32_t));

    return stream;
}


std::ifstream &operator >> (std::ifstream &stream, idk::__tex_file_t &tex)
{
    size_t strsize;
    stream.read(reinterpret_cast<char *>(&strsize), sizeof(size_t));
    
    tex.name = new char[strsize];
    stream.read(reinterpret_cast<char *>(tex.name), sizeof(char)*strsize);
    
    stream.read(reinterpret_cast<char *>(&tex.w), sizeof(size_t));
    stream.read(reinterpret_cast<char *>(&tex.h), sizeof(size_t));
    
    tex.data = new uint32_t[tex.w*tex.h];
    stream.read(reinterpret_cast<char *>(tex.data), tex.w*tex.h*sizeof(uint32_t));

    return stream;
}


void
idk::filetools::tex_save(std::string filepath, idk::__tex_file_t &tex)
{
    std::ofstream stream(filepath, std::ios_base::binary);
    stream << tex;
    stream.close();
}


void
idk::filetools::tex_load(std::string filepath, idk::__tex_file_t &tex)
{
    std::ifstream stream(filepath, std::ios_base::binary);
    stream >> tex;
    stream.close();
}


void
idk::filetools::texpak_save(std::string filepath, idk::__texpak_file_t &texpak)
{
    std::ofstream stream(filepath, std::ios_base::binary);

    size_t num_texfiles = texpak.texfiles.size();
    stream.write(reinterpret_cast<const char *>(&num_texfiles), sizeof(size_t));

    for (idk::__tex_file_t &tex: texpak.texfiles)
    {
        stream << tex;
    }

    stream.close();
}


void
idk::filetools::texpak_load(std::string filepath, idk::__texpak_file_t &texpak)
{
    std::ifstream stream(filepath, std::ios_base::binary);

    size_t num_texfiles;
    stream.read(reinterpret_cast<char *>(&num_texfiles), sizeof(size_t));
    texpak.texfiles.resize(num_texfiles);    

    for (size_t i=0; i<num_texfiles; i++)
    {
        stream >> texpak.texfiles[i];
    }

    stream.close();
}



void
idk::filetools::mat_save(std::string filepath, idk::__mat_file_t &mat)
{
    std::ofstream stream(filepath, std::ios_base::binary);

    stream.write((const char *)(mat.diff), 3*sizeof(float));
    stream.write((const char *)(mat.spec), 3*sizeof(float));
    stream.write((const char *)(&mat.spec_exp), sizeof(float));

    stream.close();
}


void
idk::filetools::mat_load(std::string filepath, idk::__mat_file_t &mat)
{
    std::ifstream stream(filepath, std::ios_base::binary);
    stream.close();
}


idk::__mat_file_t
idk::filetools::mat_cast( idk::Material &material)
{
    idk::__mat_file_t mat;

    mat.diff[0] = material.diffuse_color.x;
    mat.diff[1] = material.diffuse_color.y;
    mat.diff[2] = material.diffuse_color.z;

    mat.spec[0] = material.specular_color.x;
    mat.spec[1] = material.specular_color.y;
    mat.spec[2] = material.specular_color.z;

    mat.spec_exp = material.specular_exponent;

    return mat;
}


idk::Material
idk::filetools::mat_cast( idk::__mat_file_t &mat )
{
    idk::Material material;

    material.diffuse_color = glm::vec3(mat.diff[1], mat.diff[1], mat.diff[2]);
    material.specular_color = glm::vec3(mat.spec[1], mat.spec[1], mat.spec[2]);
    material.specular_exponent = mat.spec_exp;

    return material;
}


void
idk::filetools::vts_save(std::string filepath, idk::__vts_file_t &vts)
{
    std::ofstream stream(filepath, std::ios_base::binary);
    stream.write(reinterpret_cast<const char *>(&vts.size), sizeof(size_t));
    stream.write(reinterpret_cast<const char *>(vts.data),  vts.size*sizeof(idk::Vertex));
    stream.close();
}


void
idk::filetools::vts_load(std::string filepath, idk::__vts_file_t &vts)
{
}


void
idk::filetools::mdl_save(std::string filepath, idk::__mdl_file_t &mdl)
{
    std::ofstream stream(filepath);
    
    for (size_t i=0; i<mdl.tex_filepaths.size(); i++)
    {
        stream << mdl.tex_filepaths[i] << " " << mdl.vts_filepaths[i] << std::endl;
    }
}


void
idk::filetools::mdl_load(std::string filepath, idk::__mdl_file_t &mdl)
{

}


idk::__tex_file_t
idk::filetools::texFromIMG( std::string filepath )
{
    SDL_Surface *img = IMG_Load(filepath.c_str());

    std::string path = filepath.substr(filepath.find("assets/"));
    char *name = new char[path.size()];
    strcpy(name, path.c_str());

    return { name, (size_t)img->w, (size_t)img->h, (uint32_t *)(img->pixels) };
}

