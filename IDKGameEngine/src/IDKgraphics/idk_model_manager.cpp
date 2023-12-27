#include "idk_model_manager.hpp"

#include <fstream>
#include <sstream>
#include <set>
#include <filesystem>

#include <libidk/IDKgl.hpp>




GLuint do_thing_r( float f )
{
    constexpr size_t size = 32;
    float *data = new float[size*size];

    for (size_t i=0; i<size*size; i++)
    {
        data[i] = f;
    }

    GLuint texture;

    idk::gl::genTextures(1, &texture);
    idk::gl::bindTexture(GL_TEXTURE_2D, texture);

    idk::gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    idk::gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    idk::gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    idk::gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    idk::gl::texImage2D(GL_TEXTURE_2D, 0, GL_RED, size, size, 0, GL_RED, GL_FLOAT, data);
    idk::gl::generateMipmap(GL_TEXTURE_2D);

    idk::gl::bindTexture(GL_TEXTURE_2D, 0);

    delete[] data;
    return texture;
}


GLuint do_thing_rgb( glm::vec3 v )
{
    constexpr size_t size = 32;
    glm::vec3 *data = new glm::vec3[size*size];

    for (size_t i=0; i<size*size; i++)
    {
        data[i] = v;
    }

    GLuint texture;

    idk::gl::genTextures(1, &texture);
    idk::gl::bindTexture(GL_TEXTURE_2D, texture);

    idk::gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    idk::gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    idk::gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    idk::gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    idk::gl::texImage2D(GL_TEXTURE_2D, 0, GL_RGB8, size, size, 0, GL_RGB, GL_FLOAT, data);
    idk::gl::generateMipmap(GL_TEXTURE_2D);

    idk::gl::bindTexture(GL_TEXTURE_2D, 0);

    delete[] data;
    return texture;
}


void
idk::ModelSystem::init()
{
    m_default_albedo = do_thing_rgb(glm::vec3(0.5f));
    m_default_ao_r_m = do_thing_rgb(glm::vec3(1.0f, 0.75f, 0.0f));
    m_default_height = do_thing_r(0.0f);
    m_default_normal = do_thing_rgb(glm::vec3(0.5f, 0.5f, 1.0f));

    m_default_albedo_config = {
        .internalformat = GL_SRGB8_ALPHA8,
        .format         = GL_RGBA,
        .minfilter      = GL_LINEAR_MIPMAP_LINEAR,
        .magfilter      = GL_LINEAR,
        .wrap_s         = GL_REPEAT,
        .wrap_t         = GL_REPEAT,
        .datatype       = GL_UNSIGNED_BYTE,
        .anisotropic    = GL_TRUE,
        .genmipmap      = GL_TRUE
    };

    m_default_lightmap_config = {
        .internalformat = GL_RGBA16,
        .format         = GL_RGBA,
        .minfilter      = GL_LINEAR_MIPMAP_LINEAR,
        .magfilter      = GL_LINEAR,
        .wrap_s         = GL_REPEAT,
        .wrap_t         = GL_REPEAT,
        .datatype       = GL_UNSIGNED_BYTE,
        .anisotropic    = GL_TRUE,
        .genmipmap      = GL_TRUE
    };

}


int
idk::ModelSystem::new_material()
{
    int material_id = m_materials.create();
    idk::Material &material = m_materials.get(material_id);

    material.name            = "No idea lmao";
    material.albedo_id       = m_default_albedo;
    material.arm_id          = m_default_ao_r_m;
    material.displacement_id = m_default_height;
    material.normal_id       = m_default_normal;

    return material_id;
}


void
idk::ModelSystem::loadTexture( std::string filepath, const glTextureConfig &config )
{
    GLuint tex_id = gltools::loadTexture(filepath, config);

    // Textures are referenced using their relative path
    std::string relpath = std::filesystem::path(filepath).relative_path();

    m_texture_IDs[relpath].set = true;
    m_texture_IDs[relpath].texture_ID = tex_id;
}


void
idk::ModelSystem::model_to_gpu( idk::Model &model )
{
    gl::genVertexArrays(1, &model.VAO);
    gl::genBuffers(1, &model.VBO);
    gl::genBuffers(1, &model.IBO);

    gl::bindVertexArray(model.VAO);
    gl::bindBuffer(GL_ARRAY_BUFFER, model.VBO);

    size_t vertex_size = model.m_vertices->typesize();

    gl::bufferData(
        GL_ARRAY_BUFFER,
        model.m_vertices->nbytes(),
        model.m_vertices->data(),
        GL_STATIC_DRAW
    );

    gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.IBO);
    gl::bufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        model.m_indices.size() * sizeof(GLuint),
        model.m_indices.data(),
        GL_STATIC_DRAW
    );


    GLuint offset = 0;

    // Position
    gl::vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, offset);
    gl::enableVertexAttribArray(0);
    offset += 3 * sizeof(float);

    // Normal
    gl::vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_size, offset);
    gl::enableVertexAttribArray(1);
    offset += 3 * sizeof(float);

    // Tangent
    gl::vertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertex_size, offset);
    gl::enableVertexAttribArray(2);
    offset += 3 * sizeof(float);

    // UV
    gl::vertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, vertex_size, offset);
    gl::enableVertexAttribArray(3);
    offset += 2 * sizeof(float);

    if (model.render_flags & ModelRenderFlag::ANIMATED)
    {
        // Bone IDs
        gl::vertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, vertex_size, offset);
        gl::enableVertexAttribArray(4);
        offset += 4 * sizeof(int);

        // Bone weights
        gl::vertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, vertex_size, offset);
        gl::enableVertexAttribArray(5);
        offset += 4 * sizeof(float);
    }

    gl::bindVertexArray(0);
    gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void
idk::ModelSystem::instanced_to_gpu( idk::Model &model, const std::vector<glm::mat4> &transforms )
{
    gl::genVertexArrays(1, &model.VAO);
    gl::genBuffers(1, &model.VBO);
    gl::genBuffers(1, &model.IBO);

    gl::bindVertexArray(model.VAO);
    gl::bindBuffer(GL_ARRAY_BUFFER, model.VBO);


    size_t vertex_size = sizeof(idk::Vertex);

    gl::bufferData(
        GL_ARRAY_BUFFER,
        model.m_vertices->nbytes(),
        model.m_vertices->data(),
        GL_STATIC_DRAW
    );

    gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.IBO);
    gl::bufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        model.m_indices.size() * sizeof(GLuint),
        model.m_indices.data(),
        GL_STATIC_DRAW
    );


    GLuint index  = 0;
    GLuint offset = 0;

    // Position
    gl::vertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, vertex_size, offset);
    gl::enableVertexAttribArray(0);
    index  += 1;
    offset += 3 * sizeof(float);

    // Normal
    gl::vertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, vertex_size, offset);
    gl::enableVertexAttribArray(1);
    index  += 1;
    offset += 3 * sizeof(float);

    // Tangent
    gl::vertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, vertex_size, offset);
    gl::enableVertexAttribArray(2);
    index  += 1;
    offset += 3 * sizeof(float);

    // UV
    gl::vertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, vertex_size, offset);
    gl::enableVertexAttribArray(3);
    index  += 1;
    offset += 2 * sizeof(float);


    // Send data for instancing
    model.m_instancedata.init(index, transforms);


    gl::bindVertexArray(0);
    gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


int
idk::ModelSystem::loadModel( const std::string &root, const std::string &name )
{
    idkvi_header_t header = filetools::readheader(root+name+".txt");

    int model_id = m_models.create();
    idk::Model &model = getModel(model_id);

    std::ifstream stream(root+name+".idkvi", std::ios::binary);

    if (header.animated)
    {
        model.render_flags |= ModelRenderFlag::ANIMATED;
        model.animator_id = createAnimator();
        model.m_vertices  = new idk::Buffer<idk::AnimatedVertex>();

        filetools::readidkvi(stream, header, model.m_vertices, model.m_indices);
        filetools::readidka(stream, header, m_animators.get(model.animator_id));
    }
    else
    {
        model.m_vertices = new idk::Buffer<idk::Vertex>();
    
        filetools::readidkvi(stream, header, model.m_vertices, model.m_indices);
    }
    stream.close();

    for (size_t i=0; i<header.num_meshes; i++)
    {
        model.meshes.push_back(idk::Mesh());
        idk::Mesh &mesh   = model.meshes.back();
        mesh.num_indices  = header.m_index_counts[i];

        int material_id  = new_material();
        mesh.material_id = material_id;
        idk::Material &material = m_materials.get(material_id);

        auto &bitmask  = header.m_bitmasks[i];
        auto &textures = header.m_texture_paths[i];

        if (bitmask & ALBEDO_BIT)
            material.albedo_id = get_texture_id(textures[ALBEDO_IDX], m_default_albedo_config);

        if (bitmask & NORMAL_BIT)
            material.normal_id = get_texture_id(textures[NORMAL_IDX], m_default_lightmap_config);

        if (bitmask & RM_BIT)
            material.arm_id = get_texture_id(textures[RM_IDX], m_default_lightmap_config);
    }

    model_to_gpu(model);

    return model_id;
}


GLuint
idk::ModelSystem::get_texture_id( const std::string &key, const glTextureConfig &config )
{
    if (m_texture_IDs[key].set == false)
    {
        loadTexture(key, config);
    }

    return m_texture_IDs[key].texture_ID;
}



int
idk::ModelSystem::createAnimator()
{
    return m_animators.create();
}



int
idk::ModelSystem::copyAnimator( int model_id )
{
    int animator_id    = getModel(model_id).animator_id;
    Animator &animator = getAnimator(animator_id);

    return m_animators.create(animator);
}


int
idk::ModelSystem::createInstancedModel( int model_id, const std::vector<glm::mat4> &transforms )
{
    int   id    = m_models.create(m_models.get(model_id));
    auto &model = m_models.get(id);
    model.render_flags |= ModelRenderFlag::INSTANCED;

    instanced_to_gpu(model, transforms);

    return id;
}


int
idk::ModelSystem::createChunkedModel( int model_id, const std::vector<glm::vec4> &positions,
                                      const std::vector<idk::OBB> &OBBs,
                                      const std::vector<std::vector<glm::mat4>> &transforms )
{
    int  chunked_id = m_models.create(m_models.get(model_id));
    auto &model     = m_models.get(chunked_id);
    model.render_flags |= ModelRenderFlag::CHUNKED;

    idk::loadChunked(model, positions, OBBs, transforms);
    instanced_to_gpu(model, model.m_chunk_transforms);

    return chunked_id;
}

