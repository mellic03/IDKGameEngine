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
    m_default_metallic  = do_thing_r(0.0f);
    m_default_roughness = do_thing_r(0.7f);
    m_default_height    = do_thing_r(0.0f);
    m_default_ao        = do_thing_r(1.0f);
    m_default_normal    = do_thing_rgb(glm::vec3(0.5f, 0.5f, 1.0f));
}



static void charreplace(std::string &str, char from, char to)
{
    for (char &c: str)
        if (c == from)
            c = to;
}


static void charreplace(std::vector<std::string> &strs, char from, char to)
{
    for (std::string &str: strs)
        charreplace(str, from, to);
}


void
idk::ModelManager::_load_mtl( std::string raw_mtl )
{
    std::stringstream stream(raw_mtl);
    std::string line;
    int material_id = -1;

    while (getline(stream, line))
    {
        std::istringstream iss(line);
        std::string dummy;

        if (line.find("newmtl") != std::string::npos)
        {
            material_id = m_materials.create();
            std::string material_name;
            iss >> dummy >> material_name;
    
            _material_IDs[material_name] = material_id;

            m_materials.get(material_id).name            = material_name;
            m_materials.get(material_id).albedo_id       = m_default_metallic;
            m_materials.get(material_id).metallic_id     = m_default_metallic;
            m_materials.get(material_id).roughness_id    = m_default_roughness;
            m_materials.get(material_id).displacement_id = m_default_height;
            m_materials.get(material_id).ao_id           = m_default_ao;
            m_materials.get(material_id).normal_id       = m_default_normal;
        }

        else if (line.find("map_Ao") != std::string::npos)
        {
            size_t root = line.find("assets/");
            line = line.substr(root);
            m_materials.get(material_id).ao_id = get_texture_id(line, false);
        }

        else if (line.find("map_Ke") != std::string::npos)
        {
            size_t root = line.find("assets/");
            line = line.substr(root);
            m_materials.get(material_id).displacement_id = get_texture_id(line, false);
        }

        else if (line.find("map_Bump") != std::string::npos) // normal
        {
            size_t bm = line.find(" -bm ");
            size_t root = line.find("assets/");

            float strength = std::stof(line.substr(bm+5, 5));
            m_materials.get(material_id).normal_strength = strength;

            line = line.substr(root);
            m_materials.get(material_id).normal_id = get_texture_id(line, false);
        }

        else if (line.find("map_Kd") != std::string::npos) // albedo
        {
            size_t root = line.find("assets/");
            line = line.substr(root);
            m_materials.get(material_id).albedo_id = get_texture_id(line, false);
        }

        else if (line.find("map_Pm") != std::string::npos) // metallic
        {
            size_t root = line.find("assets/");
            line = line.substr(root);
            m_materials.get(material_id).metallic_id = get_texture_id(line, false);
        }

        else if (line.find("map_Pr") != std::string::npos) // roughness
        {
            size_t root = line.find("assets/");
            line = line.substr(root);
            m_materials.get(material_id).roughness_id = get_texture_id(line, false);
        }
    }

}


void computeTangent( idk::Vertex &v1, idk::Vertex &v2, idk::Vertex &v3 )
{
    glm::vec3 p1 = v1.position,  p2 = v2.position,  p3 = v3.position;
    glm::vec2 t1 = v1.texcoords, t2 = v2.texcoords, t3 = v3.texcoords;

    glm::vec3 edge1    = p2 - p1;
    glm::vec3 edge2    = p3 - p1;
    glm::vec2 deltaUV1 = t2 - t1;
    glm::vec2 deltaUV2 = t3 - t1;

    float d = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;
    float f = 1.0f / d;

    if (isnan(f))
    {
        f = 100000.0f;
    }

    glm::vec3 tangent = {
        f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
        f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
        f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
    };

    tangent = glm::normalize(tangent);

    v1.tangent = tangent;
    v2.tangent = tangent;
    v3.tangent = tangent;
}


idk::Model
idk::ModelManager::_load_obj( std::string raw_obj )
{
    idk::Model model;

    std::string current_material_name = "NONE";
    std::set<std::string> material_names;
    std::unordered_map<std::string, idk::Mesh> meshes;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;

    std::stringstream stream(raw_obj);
    std::string line;

    while (std::getline(stream, line))
    {
        std::istringstream iss(line);
        std::string dummy;

        if (line[0] == 'v' && line[1] == ' ')
        {
            float x, y, z;
            iss >> dummy >> x >> y >> z;
            positions.push_back(glm::vec3(x, y, z));
        }

        else if (line[0] == 'v' && line[1] == 'n')
        {
            float x, y, z;
            iss >> dummy >> x >> y >> z;
            normals.push_back(glm::vec3(x, y, z));
        }

        else if (line[0] == 'v' && line[1] == 't')
        {
            float u, v;
            iss >> dummy >> u >> v;
            texcoords.push_back(glm::vec2(u, 1.0f - v)); 
        }

        else if (line.find("usemtl") != std::string::npos)
        {
            std::string material_name;
            iss >> dummy >> material_name;

            if (material_names.find(material_name) == material_names.end())
            {
                material_names.emplace(material_name);
                meshes[material_name] = idk::Mesh();
                meshes[material_name].material_id = _material_IDs[material_name];
            }
            current_material_name = material_name;
        }

        else if (line[0] == 'f' && line[1] == ' ')
        {
            std::vector<std::string> vstrs(3);
            iss >> dummy >> vstrs[0] >> vstrs[1] >> vstrs[2];

            charreplace(vstrs, '/', ' ');

            for (size_t i=0; i<3; i++)
            {
                iss = std::istringstream(vstrs[i]);
                
                size_t pos, uv, norm;
                iss >> pos >> uv >> norm;

                idk::Vertex vert;
                vert.position  = positions[pos - 1];
                vert.texcoords = texcoords[uv - 1];
                vert.normal    = normals[norm - 1];

                meshes[current_material_name].vertex_indices.push_back(model.vertices.size());
                model.vertices.push_back(vert);
            }

            size_t start = model.vertices.size() - 3;
            computeTangent(
                model.vertices[start], model.vertices[start+1], model.vertices[start+2]
            );
        }
    }

    for (auto &[name, mesh]: meshes)
    {
        model.meshes.push_back(mesh);
    }


    return model;
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


int
idk::ModelManager::loadOBJ( std::string raw_obj, std::string raw_mtl )
{
    _load_mtl(raw_mtl);
    int model_id    = m_models.create(_load_obj(raw_obj));
    int instance_id = m_instancedata.create();
    
    idk::Model &model = m_models.get(model_id);

    gl::genVertexArrays(1, &model.VAO);
    gl::genBuffers(1, &model.VBO);

    gl::bindVertexArray(model.VAO);
    gl::bindBuffer(GL_ARRAY_BUFFER, model.VBO);

    gl::bufferData(
        GL_ARRAY_BUFFER,
        model.vertices.size() * sizeof(idk::Vertex),
        &(model.vertices[0]),
        GL_STATIC_DRAW
    );

    unsigned long offset = 0;

    // Position
    gl::vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(idk::Vertex), offset);
    gl::enableVertexAttribArray(0);
    offset += 3 * sizeof(float);

    // Normal
    gl::vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(idk::Vertex), offset);
    gl::enableVertexAttribArray(1);
    offset += 3 * sizeof(float);

    // Tangent
    gl::vertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(idk::Vertex), offset);
    gl::enableVertexAttribArray(2);
    offset += 3 * sizeof(float);

    // UV
    gl::vertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(idk::Vertex), offset);
    gl::enableVertexAttribArray(3);
    offset += 2 * sizeof(float);


    for (int i=0; i<model.meshes.size(); i++)
    {
        idk::Mesh &mesh = model.meshes[i];

        gl::genBuffers(1, &mesh.IBO);
        gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
        gl::bufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            mesh.vertex_indices.size() * sizeof(GLuint),
            &mesh.vertex_indices[0],
            GL_STATIC_DRAW
        );
    }

    m_instancedata.get(model_id).init(16);

    gl::bindVertexArray(0);

    return model_id;
}


int
idk::ModelManager::loadOBJ( std::string rootpath, std::string obj, std::string mtl )
{
    std::stringstream obj_buf, mtl_buf;
    obj_buf << std::ifstream(rootpath + obj).rdbuf();
    mtl_buf << std::ifstream(rootpath + mtl).rdbuf();
    return loadOBJ(obj_buf.str(), mtl_buf.str());
}


void
idk::ModelManager::loadVertices( std::string filepath, std::vector<idk::Vertex> &vertices )
{
    vertices.clear();

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;

    std::ifstream stream(filepath);
    std::string line;

    while (std::getline(stream, line))
    {
        std::istringstream iss(line);
        std::string dummy;

        if (line[0] == 'v' && line[1] == ' ')
        {
            float x, y, z;
            iss >> dummy >> x >> y >> z;
            positions.push_back(glm::vec3(x, y, z));
        }

        else if (line[0] == 'v' && line[1] == 'n')
        {
            float x, y, z;
            iss >> dummy >> x >> y >> z;
            normals.push_back(glm::vec3(x, y, z));
        }

        else if (line[0] == 'v' && line[1] == 't')
        {
            float u, v;
            iss >> dummy >> u >> v;
            texcoords.push_back(glm::vec2(u, 1.0f - v)); 
        }

        else if (line[0] == 'f' && line[1] == ' ')
        {
            std::vector<std::string> vstrs(3);
            iss >> dummy >> vstrs[0] >> vstrs[1] >> vstrs[2];

            charreplace(vstrs, '/', ' ');

            for (size_t i=0; i<3; i++)
            {
                iss = std::istringstream(vstrs[i]);
                
                size_t pos, uv, norm;
                iss >> pos >> uv >> norm;

                idk::Vertex vert;
                vert.position  = positions[pos - 1];
                vert.texcoords = texcoords[uv - 1];
                vert.normal    = normals[norm - 1];

                vertices.push_back(vert);
            }

        }
    }
}


GLuint
idk::ModelManager::get_texture_id( const std::string &key, bool srgb )
{
    if (_texture_IDs[key].set == false)
    {
        loadTexture(key, srgb, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
        // return m_default_metallic;
    }

    return _texture_IDs[key].texture_ID;
}


