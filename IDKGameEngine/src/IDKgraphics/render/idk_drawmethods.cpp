#include "idk_drawmethods.hpp"


void
idk::drawmethods::bind_material( const std::string &name, glShader &program, Material &material )
{
    program.set_sampler2D ( name + ".albedo",       material.albedo_id       );
    program.set_sampler2D ( name + ".rough_metal",  material.arm_id          );
    program.set_sampler2D ( name + ".displacement", material.displacement_id );
    program.set_sampler2D ( name + ".normal",       material.normal_id       );

    program.set_float ( name + ".metallic_strength",     material.metallic_strength     );
    program.set_float ( name + ".roughness_strength",    material.roughness_strength    );
    program.set_float ( name + ".displacement_strength", material.displacement_strength );
    program.set_float ( name + ".normal_strength",       material.normal_strength       );
}


void
idk::drawmethods::bind_material( glShader &program, Material &material )
{
    bind_material("un_material", program, material);
}



void
idk::drawmethods::dummy( glShader &, int, const glm::mat4 &, ModelSystem & )
{

}


void
idk::drawmethods::draw_textured( glShader &program, int model_id,
                                 const glm::mat4 &transform, ModelSystem &MS )
{
    idk::Model &model = MS.getModel(model_id);
    program.set_mat4("un_model", transform);

    gl::bindVertexArray(model.VAO);
    uint64_t start_idx = 0;

    if (model.render_flags & ModelRenderFlag::INSTANCED)
    {
        for (idk::Mesh &mesh: model.meshes)
        {
            auto &material = MS.getMaterial(mesh.material_id);
            program.set_int("un_material_id", material.bindless_idx);

            gl::drawElementsInstanced(
                GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT,
                (void *)(start_idx), model.m_instancedata.instancecount
            );

            start_idx += mesh.num_indices * sizeof(GLuint);
        }
    }

    else
    {
        for (idk::Mesh &mesh: model.meshes)
        {
            auto &material = MS.getMaterial(mesh.material_id);
            program.set_int("un_material_id", material.bindless_idx);

            gl::drawElements(
                GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT,
                (void *)(start_idx)
            );

            start_idx += mesh.num_indices * sizeof(GLuint);
        }
    }
}



void
idk::drawmethods::draw_animated( float dtime, glUBO &UBO, int animator_id, int model_id,
                                 glShader &program, glm::mat4 &model_mat, ModelSystem &MS )
{
    static std::vector<glm::mat4> transforms;

    Animator &animator = MS.getAnimator(animator_id);
    animator.tick(dtime);
    animator.computeTransforms(transforms);

    UBO.bufferSubData(0, transforms.size()*sizeof(glm::mat4), transforms.data());

    Model &model = MS.getModel(model_id);

    draw_textured(program, model_id, model_mat, MS);
}



void
idk::drawmethods::draw_untextured( glShader &program, int model_id,
                                   const glm::mat4 &model_mat, ModelSystem &MS )
{
    idk::Model &model = MS.getModel(model_id);
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



void
idk::drawmethods::draw_instanced( glShader &program, int model_id,
                                  const glm::mat4 &transform, ModelSystem &MS )
{
    idk::Model &model = MS.getModel(model_id);
    auto &material = MS.getMaterial(model.meshes[0].material_id);
    program.set_int("un_material_id", material.bindless_idx);

    gl::bindVertexArray(model.VAO);
    gl::bindBuffer(GL_DRAW_INDIRECT_BUFFER, model.m_IDB);

    gl::multiDrawElementsIndirect(
        GL_TRIANGLES,
        GL_UNSIGNED_INT,
        nullptr,
        model.m_draw_params.size(),
        sizeof(idk::glDrawElementsIndirectCommand)
    );


    gl::bindVertexArray(0);
}



void
idk::drawmethods::draw_indirect( glShader &program, int model_id,
                                 const glm::mat4 &transform, ModelSystem &MS )
{
    idk::Model &model = MS.getModel(model_id);

    auto &material = MS.getMaterial(model.meshes[0].material_id);
    program.set_int("un_material_id", material.bindless_idx);

    gl::bindVertexArray(model.VAO);
    gl::bindBuffer(GL_DRAW_INDIRECT_BUFFER, model.m_IDB);

    gl::multiDrawElementsIndirect(
        GL_TRIANGLES,
        GL_UNSIGNED_INT,
        nullptr,
        model.m_draw_params.size(),
        sizeof(idk::glDrawElementsIndirectCommand)
    );


    gl::bindVertexArray(0);
}


void
idk::drawmethods::draw_heightmapped( glShader &program, int model_id,
                                     const glm::mat4 &transform, ModelSystem &MS )
{
    idk::Model &model = MS.getModel(model_id);
    idk::Model_Terrain &terrain = MS.getModel_Terrain(model.terrain_id);

    program.set_mat4("un_model", transform);

    program.set_Handleui64ARB("un_heightmap", terrain.heightmap_handle);

    program.set_float("un_height_scale", terrain.height_scale);
    program.set_float("un_world_scale",  terrain.world_scale);


    for (size_t i=0; i<terrain.num_materials; i++)
    {
        idk::Material &material = MS.getMaterial(terrain.material_ids[i]);
        bind_material("un_material_" + std::to_string(i), program, material);
    }

    program.popTextureUnits();

    gl::bindVertexArray(model.VAO);
    gl::drawElements(GL_TRIANGLES, model.m_indices.size(), GL_UNSIGNED_INT, 0);
    gl::bindVertexArray(0);
}



