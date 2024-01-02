#include "idk_model.hpp"


void
idk::loadChunked( idk::Model &model, const std::vector<glm::vec4> &positions,
                  const std::vector<std::vector<glm::mat4>> &transforms )
{
    model.m_chunk_positions = positions;
    model.m_chunk_transforms.clear();


    GLuint base_instance = 0;

    for (const auto &v: transforms)
    {
        GLuint num_instances = v.size();

        if (num_instances == 0)
        {
            continue;
        }

        for (const glm::mat4 &M: v)
        {
            model.m_chunk_transforms.push_back(M);
        }

        model.m_default_draw_params.push_back({
            .count          = static_cast<GLuint>(model.m_indices.size()),
            .instanceCount  = static_cast<GLuint>(num_instances),
            .firstIndex     = 0,
            .baseVertex     = 0,
            .baseInstance   = base_instance 
        });

        base_instance += num_instances;
    }

    gl::createBuffers(1, &model.m_IDB);
    gl::namedBufferData(
        model.m_IDB,
        model.m_default_draw_params.size() * sizeof(idk::glDrawElementsIndirectCommand),
        // reinterpret_cast<const void *>(model.m_default_draw_params.data()),
        nullptr,
        GL_DYNAMIC_DRAW
    );
}



void idk::genDrawCommands( idk::Model &model, const glm::mat4 &transform,
                           float cam_near, float cam_far,
                           const glm::mat4 &cam_proj, const glm::mat4 &cam_view )
{
    model.m_draw_params.clear();

    for (size_t i=0; i<model.m_chunk_positions.size(); i++)
    {
        glm::vec3 objectspace = model.m_chunk_positions[i];
        float radius = model.m_chunk_positions[i].w;

        glm::vec4 viewspace = cam_view * glm::vec4(objectspace, 1.0f);

        if (viewspace.z > cam_far)
        {
            continue;
        }


        glm::vec4 screenspace = cam_proj * viewspace;
                  screenspace /= screenspace.w;

        if (fabs(screenspace.x) > 1.0f || fabs(screenspace.y) > 1.0f || fabs(screenspace.z) > 1.0f)
        {
            continue;
        }

        auto &default_param = model.m_default_draw_params[i];
        model.m_draw_params.push_back(default_param);
    }

    gl::namedBufferSubData(
        model.m_IDB,
        0,
        model.m_draw_params.size() * sizeof(idk::glDrawElementsIndirectCommand),
        reinterpret_cast<const void *>(model.m_draw_params.data())
    );

    // std::cout << "num rendered: "<< model.m_draw_params.size() << "\n";
}


