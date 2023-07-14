#include "idk_tools.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>


int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout
            << "Usage: <img> <idktex>\n"
            << "  img       image filepath\n"
            << "  idxtex    output .idktex filepath\n"
            << std::endl;

        return 1;
    }

    std::filesystem::path rootpath(argv[1]);
    for (auto const &dir_entry: std::filesystem::recursive_directory_iterator{rootpath})
    {
        if (dir_entry.is_directory())
            continue;

        idk::__tex_file_t texfile = idk::filetools::loadImage(dir_entry.path().string());

        std::string outputpath = dir_entry.path().string();
        outputpath = outputpath.substr(outputpath.find("/textures") + 10);
        outputpath = std::string(argv[2]) + outputpath;
        std::cout << outputpath << std::endl;

        idk::filetools::tex_save(outputpath, texfile);
    }



    return 0;
}
