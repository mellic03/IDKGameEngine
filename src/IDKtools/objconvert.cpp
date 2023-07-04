#include "idk_tools.h"

#include <iostream>
#include <fstream>
#include <string>


int main(int argc, char **argv)
{

    if (argc != 5)
    {
        std::cout
            << "Usage: <root> <obj> <mtl> <output>\n"
            << "  root      -root filepath\n"
            << "  obj       -obj filepath\n"
            << "  mtl       -mtl filepath\n"
            << "  output    -output directory\n"
            << std::endl;

        return 1;
    }

    /*
        diff spec spec w h 1010101 w h 1001101 w h 100101101
    */

    // idk::RenderEngine ren(1, 1);

    // std::string root(argv[1]);
    // std::string obj(argv[2]);
    // std::string mtl(argv[3]);
    // std::string outputdir(argv[4]);


    // ren.loadTextures("assets/textures/");

    // int model_id = ren.modelManager().loadOBJ(root, obj, mtl);
    // idk::Model &model = ren.modelManager().getModel(model_id);

    // // Write vertex data ------------------------------------------------------------
    // std::vector<idk::Vertex> &vertices = model.vertices;
    // idk::filetools::vector_to_bin(outputdir + "modelname.vts", model.vertices);
    // // ------------------------------------------------------------------------------

    // auto materials = ren.modelManager().getMaterials();

    // for (int i=0; i <model.meshes.size(); i++)
    // {
    //     idk::Mesh &mesh = model.meshes[i];
    //     idk::Material &material = materials.get(mesh.material_id);

    //     // Write material to file
    //     idk::__mat_file_t mat = idk::filetools::mat_cast(material);
    //     idk::filetools::mat_save(
    //         outputdir + std::to_string(i) + ".mat",
    //         mat
    //     );

    //     // Write index data to file
    //     std::vector<GLuint> &indices = mesh.vertex_indices;
    //     idk::filetools::vector_to_bin(
    //         outputdir + std::to_string(i) + ".idx",
    //         indices
    //     );
    // }

    return 0;
}
