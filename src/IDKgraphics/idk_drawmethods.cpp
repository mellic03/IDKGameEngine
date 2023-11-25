#include "idk_drawmethods.h"

void
idk::drawmethods::bind_material( glShader &program, Material &material )
{
    program.set_sampler2D ( "un_albedo_texture",     material.albedo_id    );
    program.set_sampler2D ( "un_metallic_texture",   material.metallic_id  );
    program.set_sampler2D ( "un_roughness_texture",  material.roughness_id );
    program.set_sampler2D ( "un_ao_texture",         material.ao_id        );
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
