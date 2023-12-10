#include "idk_model_manager.hpp"

#include <fstream>
#include <sstream>
#include <set>
#include <filesystem>




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

    idk::gl::texImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, data);
    idk::gl::generateMipmap(GL_TEXTURE_2D);

    idk::gl::bindTexture(GL_TEXTURE_2D, 0);

    delete[] data;
    return texture;
}


void
idk::ModelManager::init()
{
    m_default_albedo = do_thing_rgb(glm::vec3(0.5f));
    m_default_rm     = do_thing_rgb(glm::vec3(0.0f, 0.7f, 0.0f));
    m_default_ao     = do_thing_r(1.0f);
    m_default_height = do_thing_r(0.0f);
    m_default_normal = do_thing_rgb(glm::vec3(0.5f, 0.5f, 1.0f));
}


int
idk::ModelManager::new_material()
{
    int material_id = m_materials.create();
    idk::Material &material = m_materials.get(material_id);

    material.name            = "No idea lmao";
    material.albedo_id       = m_default_albedo;
    material.rm_id           = m_default_rm;
    material.displacement_id = m_default_height;
    material.ao_id           = m_default_ao;
    material.normal_id       = m_default_normal;

    return material_id;
}


void
idk::ModelManager::loadTexture( std::string filepath, bool srgb, GLint minfilter, GLint magfilter )
{
    GLuint tex_id = gltools::loadTexture(filepath, srgb, minfilter, magfilter);

    // Textures are referenced using their relative path
    std::string relpath = std::filesystem::path(filepath).relative_path();

    _texture_IDs[relpath].set = true;
    _texture_IDs[relpath].texture_ID = tex_id;
}


void
idk::ModelManager::loadTextures( std::string path, bool srgb, GLint minfilter, GLint magfilter )
{
    using namespace std;
    filesystem::path directory(path);

    for (auto const &dir_entry: filesystem::recursive_directory_iterator{directory})
    {
        if (dir_entry.is_directory())
        {
            continue;
        }

        loadTexture( dir_entry.path().string(), srgb, minfilter, magfilter );
    }
}


void
idk::ModelManager::model_to_gpu( idk::Model &model )
{
    gl::genVertexArrays(1, &model.VAO);
    gl::genBuffers(1, &model.VBO);
    gl::genBuffers(1, &model.IBO);

    gl::bindVertexArray(model.VAO);

    gl::bindBuffer(GL_ARRAY_BUFFER, model.VBO);


    size_t vertex_size = sizeof(idk::Vertex);

    if (model.animated)
    {
        vertex_size = sizeof(idk::AnimatedVertex);

        gl::bufferData(
            GL_ARRAY_BUFFER,
            model.m_anim_vertices.size() * sizeof(idk::AnimatedVertex),
            model.m_anim_vertices.data(),
            GL_STATIC_DRAW
        );
    }

    else
    {
        gl::bufferData(
            GL_ARRAY_BUFFER,
            model.m_vertices.size() * sizeof(idk::Vertex),
            model.m_vertices.data(),
            GL_STATIC_DRAW
        );
    }


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

    if (model.animated)
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



int
idk::ModelManager::loadModel( const std::string &root, const std::string &name )
{
    idkvi_header_t header = filetools::readheader(root+name+".txt");

    int model_id = m_models.create();
    idk::Model &model = getModel(model_id);

    if (header.animated)
    {
        model.animated = true;

        filetools::readidkvi(
            header, root+name+".idkvi",
            model.m_anim_vertices, model.m_indices
        );
    
        filetools::readidka(header, root+name+".idka", model.m_animations);
    }

    else
    {
        model.animated = false;

        filetools::readidkvi(
            header, root+name+".idkvi",
            model.m_vertices, model.m_indices
        );
    }

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
            material.albedo_id = get_texture_id(textures[ALBEDO_IDX], false);

        if (bitmask & NORMAL_BIT)
            material.normal_id = get_texture_id(textures[NORMAL_IDX], false);

        if (bitmask & RM_BIT)
            material.rm_id = get_texture_id(textures[RM_IDX], false);
    }

    model_to_gpu(model);

    return model_id;
}



GLuint
idk::ModelManager::get_texture_id( const std::string &key, bool srgb )
{
    if (_texture_IDs[key].set == false)
    {
        loadTexture(key, srgb, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    }

    return _texture_IDs[key].texture_ID;
}


