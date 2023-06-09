#include "idk_drawmethods.h"

void
idk::drawmethods::bind_material( Material &material )
{
    glInterface::setUniform_texture("un_albedo_texture", material.albedo_gl_id);
    glInterface::setUniform_texture("un_specular_texture", material.specular_gl_id);
    glInterface::setUniform_float("un_specular_exponent", material.specular_exponent);
}


void
idk::drawmethods::draw_textured( Model &model, Transform &transform, Allocator<Material> &materials )
{
    glm::mat4 model_mat = transform.modelMatrix();
    glInterface::setUniform_mat4("un_model", model_mat);

    gl::bindVertexArray(model.VAO);
    for (int i=0; i<model.meshes.size(); i++)
    {
        Mesh &mesh = model.meshes[i];

        if (mesh.material_id != -1)
            bind_material(materials.get(mesh.material_id));

        gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
        gl::drawElements(GL_TRIANGLES, mesh.vertex_indices.size(), GL_UNSIGNED_INT, 0);
    }
}


void
idk::drawmethods::draw_untextured( Model &model, Transform &transform, Allocator<Material> &materials )
{
    glm::mat4 model_mat = transform.modelMatrix();
    glInterface::setUniform_mat4("un_model", model_mat);

    gl::bindVertexArray(model.VAO);
    for (int i=0; i<model.meshes.size(); i++)
    {
        Mesh &mesh = model.meshes[i];
        gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
        gl::drawElements(GL_TRIANGLES, mesh.vertex_indices.size(), GL_UNSIGNED_INT, 0);
    }
}
