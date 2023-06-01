#include "model.h"



void IDK::Model::_load_obj(std::string &path)
{

}


void IDK::Model::_load_mtl(std::string &path)
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



