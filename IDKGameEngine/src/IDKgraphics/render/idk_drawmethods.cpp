#include "idk_drawmethods.hpp"

void
idk::drawmethods::bind_material( glShader &program, Material &material )
{
    program.set_sampler2D ( "un_material.albedo",       material.albedo_id       );
    program.set_sampler2D ( "un_material.rough_metal",  material.arm_id          );
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

    if (model.render_flags & ModelRenderFlag::INSTANCED)
    {
        for (idk::Mesh &mesh: model.meshes)
        {
            program.popTextureUnits();

            if (mesh.material_id != -1)
                bind_material(program, materials.get(mesh.material_id));

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
}



void
idk::drawmethods::draw_animated( float dtime, glUBO &UBO, int animator_id, int model_id,
                                 glShader &program, glm::mat4 &model_mat, ModelSystem &modelsys )
{
    static std::vector<glm::mat4> transforms;

    Animator &animator = modelsys.getAnimator(animator_id);
    animator.tick(dtime);
    animator.computeTransforms(transforms);

    UBO.bind();
    UBO.add(transforms.size()*sizeof(glm::mat4), transforms.data());
    UBO.unbind();

    Model &model = modelsys.getModel(model_id);

    draw_textured(program, model, model_mat, modelsys.getMaterials());
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



void
idk::drawmethods::draw_instanced( glShader &program, int model_id, glm::mat4 &transform,
                                  ModelSystem &MS )
{
    idk::Model &model = MS.getModel(model_id);
    bind_material(program, MS.getMaterial(model.meshes[0].material_id));

    gl::bindVertexArray(model.VAO);
    gl::bindBuffer(GL_DRAW_INDIRECT_BUFFER, model.m_IDB);

    // std::cout << "\n";
    // std::cout << "vertices:         " << model.m_vertices->size() << "\n";
    // std::cout << "indices:          " << model.m_indices.size() << "\n";
    // std::cout << "chunk positions:  " << model.m_chunk_positions.size() << "\n";
    // std::cout << "chunk transforms: " << model.m_chunk_transforms.size() << "\n";
    // std::cout << "draw params:      " << model.m_draw_params.size() << "\n";
    // std::cout << "\n";
    // std::cout << model.m_draw_params[0].count << "\n";
    // std::cout << model.m_draw_params[0].instanceCount << "\n";
    // std::cout << model.m_draw_params[0].firstIndex << "\n";
    // std::cout << model.m_draw_params[0].baseVertex << "\n";
    // std::cout << model.m_draw_params[0].baseInstance << "\n";
    // std::cout << "\n";

    gl::multiDrawElementsIndirect(
        GL_TRIANGLES,
        GL_UNSIGNED_INT,
        nullptr,
        model.m_draw_params.size(),
        sizeof(idk::glDrawElementsIndirectCommand)
    );


    gl::bindVertexArray(0);
}



