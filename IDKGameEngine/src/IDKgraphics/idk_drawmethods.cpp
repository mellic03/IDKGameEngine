#include "idk_drawmethods.hpp"

void
idk::drawmethods::bind_material( glShader &program, Material &material )
{
    // program.set_sampler2D ( "un_albedo_texture",       material.albedo_id       );
    // program.set_sampler2D ( "un_metallic_texture",     material.metallic_id     );
    // program.set_sampler2D ( "un_roughness_texture",    material.roughness_id    );
    // program.set_sampler2D ( "un_ao_texture",           material.ao_id           );
    // program.set_sampler2D ( "un_displacement_texture", material.displacement_id );
    // program.set_sampler2D ( "un_normal_texture",       material.normal_id       );

    program.set_sampler2D ( "un_material.albedo",       material.albedo_id       );
    program.set_sampler2D ( "un_material.metallic",     material.metallic_id     );
    program.set_sampler2D ( "un_material.roughness",    material.roughness_id    );
    program.set_sampler2D ( "un_material.ao",           material.ao_id           );
    program.set_sampler2D ( "un_material.displacement", material.displacement_id );
    program.set_sampler2D ( "un_material.normal",       material.normal_id       );

    program.set_float ( "un_material.metallic_strength",     material.metallic_strength     );
    program.set_float ( "un_material.roughness_strength",    material.roughness_strength    );
    program.set_float ( "un_material.displacement_strength", material.displacement_strength );
    program.set_float ( "un_material.normal_strength",       material.normal_strength       );

    program.set_int( "un_material.reflectance", material.reflectance );
}


void
idk::drawmethods::draw_textured( glShader &program, Model &model, glm::mat4 &model_mat, Allocator<Material> &materials )
{
    program.set_mat4("un_model", model_mat);

    gl::bindVertexArray(model.VAO);

    for (size_t i=0; i<model.meshes.size(); i++)
    {
        Mesh &mesh = model.meshes[i];

        if (mesh.material_id != -1)
            bind_material(program, materials.get(mesh.material_id));

        gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
        gl::drawElements(GL_TRIANGLES, mesh.vertex_indices.size(), GL_UNSIGNED_INT, 0);
        // gl::drawElementsInstanced(GL_TRIANGLES, mesh.vertex_indices.size(), GL_UNSIGNED_INT, 0, 16);

        program.popTextureUnits();
    }
}


void
idk::drawmethods::draw_untextured( glShader &program, Model &model, glm::mat4 &model_mat )
{
    program.set_mat4("un_model", model_mat);

    gl::bindVertexArray(model.VAO);
    for (size_t i=0; i<model.meshes.size(); i++)
    {
        Mesh &mesh = model.meshes[i];
        gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
        gl::drawElements(GL_TRIANGLES, mesh.vertex_indices.size(), GL_UNSIGNED_INT, 0);

        program.popTextureUnits();
    }
}
