#include <fstream>
#include <sstream>
#include <istream>
#include <iostream>
#include "IDKtools.h"

idk::vector<idk::vec3> positions;
idk::vector<idk::vec3> normals;
idk::vector<idk::vec2> uvs;

void IDKtools::objconvert(
    std::string root, std::string obj,
    std::string mtl, std::string to
)
{
    std::ifstream objstream(root + obj);
    std::ifstream mtlstream(root + mtl);
    std::string line;


    while (getline(objstream, line))
    {
        std::istringstream iss(line);
        std::string dummy;

        if (line.find("v ") != std::string::npos)
        {
            float x, y, z;
            iss >> dummy >> x >> y >> z;
            positions.push(idk::vec3(x, y, z));
        }

        else if (line.find("vn ") != std::string::npos)
        {
            float x, y, z;
            iss >> dummy >> x >> y >> z;
            normals.push(idk::vec3(x, y, z));
        }

        else if (line.find("vt ") != std::string::npos)
        {
            float u, v;
            iss >> dummy >> u >> v;
            uvs.push(idk::vec2(u, v)); 
        }
    }

    std::cout << positions.size() << " " << normals.size() << "\n";
}


int main(int argc, char **argv)
{
    if (argc != 5)
    {
        std::cout
        << "Usage: objconvert [ROOT] [OBJ] [MTL] [OUT]\n"
        << "  ROOT \t path containing both obj and mtl files\n"
        << "  OBJ  \t obj filepath relative to root\n"
        << "  MTL  \t mtl filepath relative to root\n"
        << "  OUT  \t output filepath relative to working directory\n\n";
        return 1;
    }

    IDKtools::objconvert(argv[1], argv[2], argv[3], argv[4]);

    return 0;
}

