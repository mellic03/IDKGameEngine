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
            << "Usage: <inputdir> <output>\n"
            << "  inputdir      input directory\n"
            << "  output        output texpak filepath\n"
            << std::endl;

        return 1;
    }

    idk::__texpak_file_t texpak;

    std::filesystem::path rootpath(argv[1]);
    for (auto const &dir_entry: std::filesystem::directory_iterator{rootpath})
    {
        if (dir_entry.is_directory())
            continue;

        // std::cout << dir_entry.path().string() << std::endl;
        auto texfile = idk::filetools::texFromIMG(dir_entry.path().string());
        texpak.texfiles.push(texfile);
    }


    std::string outputpath = std::string(argv[2]);
    idk::filetools::texpak_save(outputpath, texpak);

    return 0;
}
