#include "model.h"


idk::Model::Model(std::string root, std::string obj, std::string mtl)
{
    
}


void idk::Model::_load_obj(std::string &path)
{

}


void idk::Model::_load_mtl(std::string &path)
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



