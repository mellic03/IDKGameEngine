#include "idk_drawmethods.hpp"

void
idk::drawmethods::bind_material( glShader &program, Material &material )
{
    program.set_sampler2D ( "un_material.albedo",       material.albedo_id       );
    program.set_sampler2D ( "un_material.rough_metal",  material.arm_id           );
    program.set_sampler2D ( "un_material.displacement", material.displacement_id );
    program.set_sampler2D ( "un_material.normal",       material.normal_id       );

    program.set_float ( "un_material.metallic_strength",     material.metallic_strength     );
    program.set_float ( "un_material.roughness_strength",    material.roughness_strength    );
    program.set_float ( "un_material.displacement_strength", material.displacement_strength );
    program.set_float ( "un_material.normal_strength",       material.normal_strength       );
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
idk::drawmethods::draw_animated( float dt, glUBO &UBO, glShader &program, Model &model,
                                 glm::mat4 &model_mat, Allocator<Material> &materials )
{
    static std::vector<glm::mat4> transforms;

    model.m_anim_controller.tick(dt);
    model.m_anim_controller.computeTransforms(transforms);

    UBO.bind();
    UBO.add(transforms.size()*sizeof(glm::mat4), transforms.data());
    UBO.unbind();

    // for (size_t i=0; i<transforms.size(); i++)
    // {
    //     program.set_mat4(
    //         "un_bonetransforms[" + std::to_string(i) + "]",
    //         transforms[i]
    //     );
    // }

    draw_textured(program, model, model_mat, materials);
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
