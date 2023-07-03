#include "idk_filetypes.h"
#include <fstream>


void
idk::filetools::tex_save(std::string filepath, idk::__tex_file_t &tex)
{
    std::ofstream stream(filepath, std::ios_base::binary);
    stream.write(reinterpret_cast<const char *>(&tex.w), sizeof(int));
    stream.write(reinterpret_cast<const char *>(&tex.h), sizeof(int));
    stream.write(reinterpret_cast<const char *>(tex.data),  tex.w*tex.h*sizeof(uint8_t));
    stream.close();
}


void
idk::filetools::tex_load(std::string filepath, idk::__tex_file_t &tex)
{
    std::ifstream stream(filepath, std::ios_base::binary);

    stream.read(reinterpret_cast<char *>(tex.w), sizeof(int));
    stream.read(reinterpret_cast<char *>(tex.h), sizeof(int));

    tex.data = new uint8_t[tex.w*tex.h];
    stream.read(reinterpret_cast<char *>(tex.data), tex.w*tex.h*sizeof(uint8_t));

    stream.close();
}


void
idk::filetools::vts_save(std::string filepath, idk::__vts_file_t &vts)
{

}


void
idk::filetools::vts_load(std::string filepath, idk::__vts_file_t &vts)
{
}


void
idk::filetools::mdl_save(std::string filepath, idk::__mdl_file_t &mdl)
{

}


void
idk::filetools::mdl_load(std::string filepath, idk::__mdl_file_t &mdl)
{

}
