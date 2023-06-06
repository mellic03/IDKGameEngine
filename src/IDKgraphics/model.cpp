#include "model.h"
#include "model.h"


static void charreplace(std::string &str, char from, char to)
{
    for (char &c: str)
        if (c == from)
            c = to;
}

static void charreplace(idk::vector<std::string> &strs, char from, char to)
{
    for (std::string &str: strs)
        charreplace(str, from, to);
}


void idk::Model::_load_obj(std::string path)
{
    std::ifstream instream(path);
    std::string line;

    idk::vector<idk::vertex> vertices;
    idk::vector<idk::vec4> positions, normals;
    idk::vector<idk::vec2> uvs;

    while (getline(instream, line))
    {
        std::istringstream iss(line);
        std::string dummy;

        if (line.find("v ") != std::string::npos)
        {
            float x, y, z;
            iss >> dummy >> x >> y >> z;
            positions.push(idk::vec4(x, y, z, 1.0f));
        }

        else if (line.find("vn ") != std::string::npos)
        {
            float x, y, z;
            iss >> dummy >> x >> y >> z;
            normals.push(idk::vec4(x, y, z, 0.0f));
        }

        else if (line.find("vt ") != std::string::npos)
        {
            float u, v;
            iss >> dummy >> u >> v;
            uvs.push(idk::vec2(u, v)); 
        }


        else if (line.find("usemtl ") != std::string::npos)
        {
            meshes.push(Mesh());
        }

        else if (line.find("f ") != std::string::npos)
        {
            idk::vector<std::string> vstrs(3);
            iss = std::istringstream(line);
            iss >> dummy >> vstrs[0] >> vstrs[1] >> vstrs[2];

            charreplace(vstrs, '/', ' ');

            for (size_t i=0; i<3; i++)
            {
                iss = std::istringstream(vstrs[i]);
                vertices.push(idk::vertex());
                
                size_t pos, norm, uv;
                iss >> pos >> norm >> uv;
                pos -= 1; norm -= 1; uv -= 1;
                vertices.back().position  = positions[pos];
                vertices.back().normal    = normals[norm];
                // vertices.back().tangent = tangent(norm);
                vertices.back().texcoords = uvs[uv];
            }

            
            // for (auto &str: vstrs)
            //     std::cout << str << "   ";
            // std::cout << std::endl;
        }
    }

    // std::cout << positions.size() << std::endl;
    // std::cout << normals.size() << std::endl;
    // std::cout << uvs.size() << std::endl;
    // std::cout << vertices.size() << std::endl;
}


void idk::Model::_load_mtl(std::string path)
{
    std::ifstream instream(path);
    std::string line;

    while(getline(instream, line))
    {
        if (line.find("newmtl") != std::string::npos)
        {
            
        }
    }

}



idk::Model::Model(std::string root, std::string obj, std::string mtl)
{
    _load_obj(root + obj);
    _load_mtl(root + mtl);
}
