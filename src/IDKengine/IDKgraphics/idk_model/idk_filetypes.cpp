#include "idk_filetypes.h"

#include <fstream>

void
idk::filetools::savetex(std::string filepath, idk::__tex_file_t &tex)
{
    std::ofstream stream(filepath, std::ios_base::binary);


    stream.close();
}


void
idk::filetools::loadtex(std::string filepath, idk::__tex_file_t &tex)
{

}


void
idk::filetools::savevts(std::string filepath, idk::__vts_file_t &vts)
{

}


void
idk::filetools::loadvts(std::string filepath, idk::__vts_file_t &vts)
{

}


void
idk::filetools::savemdl(std::string filepath, idk::__mdl_file_t &mdl)
{

}


void
idk::filetools::loadmdl(std::string filepath, idk::__mdl_file_t &mdl)
{

}



