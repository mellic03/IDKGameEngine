#include "IDK_tools.h"

#include <iomanip>



int main( int argc, char **argv )
{
    idk::RenderEngine ren(100, 100);
    
    std::string output_path = argv[1];

    std::cout << "loading model... " << std::flush;
    uint model_id = ren.loadOBJ("assets/models/", "rob.obj", "rob.mtl");
    std::cout << "Done!" << std::endl;

    idk::Model &model = ren.models().get(model_id);

    int count = 0;

    for (idk::Mesh &mesh: model.meshes)
    {
        std::vector<idk::vertex> vertices;

        for (int idx: mesh.vertex_indices)
            vertices.push_back(model.vertex_data[idx]);

        std::stringstream stream;
        stream << std::setw(8) << std::setfill('0') << count;


        std::cout << "Writing " << count << ".idkvts ...";
        idktools::vertices_to_bin(vertices, output_path + stream.str() + ".idkvts");
        std::cout << " Done!" << std::endl;

        count += 1;
    }

    return 0;
}

