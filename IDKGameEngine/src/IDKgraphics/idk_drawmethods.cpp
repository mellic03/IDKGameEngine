#include "idk_drawmethods.hpp"

void
idk::drawmethods::bind_material( glShader &program, Material &material )
{
    program.set_sampler2D ( "un_material.albedo",       material.albedo_id       );
    program.set_sampler2D ( "un_material.rough_metal",  material.rm_id           );
    program.set_sampler2D ( "un_material.ao",           material.ao_id           );
    program.set_sampler2D ( "un_material.displacement", material.displacement_id );
    program.set_sampler2D ( "un_material.normal",       material.normal_id       );

    program.set_float ( "un_material.metallic_strength",     material.metallic_strength     );
    program.set_float ( "un_material.roughness_strength",    material.roughness_strength    );
    program.set_float ( "un_material.displacement_strength", material.displacement_strength );
    program.set_float ( "un_material.normal_strength",       material.normal_strength       );

    program.set_vec3( "un_material.reflectance", material.reflectance );
}


void
idk::drawmethods::draw_textured( glShader &program, Model &model, glm::mat4 &model_mat, Allocator<Material> &materials )
{
    program.set_mat4("un_model", model_mat);

    gl::bindVertexArray(model.VAO);
    uint64_t start_idx = 0;

    for (idk::Mesh &mesh: model.meshes)
    {
        program.popTextureUnits();

        if (mesh.material_id != -1)
            bind_material(program, materials.get(mesh.material_id));

        gl::drawElements(
            GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT,
            (void *)(start_idx)
        );

        start_idx += mesh.num_indices * sizeof(GLuint);
    }
}


void
idk::drawmethods::draw_untextured( glShader &program, Model &model, glm::mat4 &model_mat )
{
    program.set_mat4("un_model", model_mat);

    gl::bindVertexArray(model.VAO);
    uint64_t start_idx = 0;

    for (idk::Mesh &mesh: model.meshes)
    {
        gl::drawElements(
            GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT,
            (void *)(start_idx)
        );

        start_idx += mesh.num_indices * sizeof(GLuint);
    }
}
