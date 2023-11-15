#include "idk_drawmethods.h"

void
idk::drawmethods::bind_material( glShader &program, Material &material )
{
    program.set_sampler2D("un_albedo_texture", material.albedo_gl_id);
    program.set_sampler2D("un_specular_texture", material.specular_gl_id);
    program.set_float("un_specular_exponent", material.specular_exponent);
    program.set_sampler2D("un_reflection_texture", material.reflection_gl_id);
}


void
idk::drawmethods::draw_textured( glShader &program, Model &model, Transform &transform, Allocator<Material> &materials )
{
    glm::mat4 model_mat = transform.modelMatrix();
    program.set_mat4("un_model", model_mat);

    gl::bindVertexArray(model.VAO);
    for (size_t i=0; i<model.meshes.size(); i++)
    {
        Mesh &mesh = model.meshes[i];

        if (mesh.material_id != -1)
            bind_material(program, materials.get(mesh.material_id));

        gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
        gl::drawElements(GL_TRIANGLES, mesh.vertex_indices.size(), GL_UNSIGNED_INT, 0);
    }
}


void
idk::drawmethods::draw_untextured( glShader &program, Model &model, Transform &transform )
{
    glm::mat4 model_mat = transform.modelMatrix();
    program.set_mat4("un_model", model_mat);

    gl::bindVertexArray(model.VAO);
    for (size_t i=0; i<model.meshes.size(); i++)
    {
        Mesh &mesh = model.meshes[i];
        gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
        gl::drawElements(GL_TRIANGLES, mesh.vertex_indices.size(), GL_UNSIGNED_INT, 0);
    }
}
