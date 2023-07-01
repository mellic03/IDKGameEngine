#include "IDK_tools.h"

#include <iomanip>
#include <fstream>
#include <sstream>


int main( int argc, char **argv )
{
    idk::RenderEngine ren(100, 100);
    ren.loadTextures("assets/textures/");

    std::string output_path = argv[1];

    std::cout << "loading model... " << std::flush;
    uint model_id = ren.loadOBJ("assets/models/", "rob.obj", "rob.mtl");
    std::cout << "Done!" << std::endl;

    idk::Model &model = ren.models().get(model_id);

    int count = 0;

    std::ofstream mdl_stream(output_path + "rob.mdl");

    for (idk::Mesh &mesh: model.meshes)
    {
        idk::Material &material = ren.glinterface().materials().get(mesh.material_id);

        unsigned char *imgdata;

        mdl_stream << std::setw(8) << std::setfill('0') << mesh.material_id << ".idktex "
        << std::setw(8) << std::setfill('0') << count << ".idkvts\n";

        std::vector<idk::Vertex> vertices;

        for (int idx: mesh.vertex_indices)
            vertices.push_back(model.vertex_data[idx]);

        std::stringstream stream;
        stream << std::setw(8) << std::setfill('0') << count;


        std::cout << "Writing " << count << ".idkvts ...";

        idk::__vts_file_t vts = idk_filetools::new_vts(vertices.size(), &vertices[0]);
        idk_filetools::vts_to_bin(vts, output_path + stream.str() + ".idkvts");

        std::cout << " Done!" << std::endl;

        count += 1;
    }

    return 0;
}

