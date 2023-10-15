#include "idk_model_manager.h"

#include <fstream>
#include <sstream>
#include <set>
#include <filesystem>


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
            material_id = _materials.create();
            std::string material_name;
            iss >> dummy >> material_name;
            _material_IDs[material_name] = material_id;
        }

        else if (line[0] == 'N' && line[1] == 's')
        {
            float Ns;
            iss >> dummy >> Ns;
            _materials.get(material_id).specular_exponent = Ns;
        }

        else if (line[0] == 'K' && line[1] == 's')
        {
            float x, y, z;
            iss >> x >> y >> z;
            _materials.get(material_id).specular_color = glm::vec3(x, y, z);
        }

        else if (line.find("map_Kd") != std::string::npos)
        {
            size_t root = line.find("assets/");
            line = line.substr(root);
            _materials.get(material_id).albedo_gl_id = _texture_IDs[line];
        }

        else if (line.find("map_Ks") != std::string::npos)
        {
            size_t root = line.find("assets/");
            line = line.substr(root);
            _materials.get(material_id).specular_gl_id = _texture_IDs[line];
        }

        else if (line.find("map_Pm") != std::string::npos)
        {
            size_t root = line.find("assets/");
            line = line.substr(root);
            _materials.get(material_id).reflection_gl_id = _texture_IDs[line];
        }
    }
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
        }
    }

    for (auto &[name, mesh]: meshes)
    {
        model.meshes.push_back(mesh);
    }


    return model;
}


void
idk::ModelManager::loadIDKtex( std::string filepath, bool srgb )
{
    __tex_file_t tex;
    filetools::tex_load(filepath, tex);

    GLuint tex_id = gltools::loadTexture(tex.w, tex.h, tex.data, srgb);
    _texture_IDs[filepath] = tex_id;
}


void
idk::ModelManager::loadIDKtexs( std::string root, bool srgb )
{
    using namespace std;

    filesystem::path rootpath(root);
    for (auto const &dir_entry: filesystem::recursive_directory_iterator{rootpath})
    {
        if (dir_entry.is_directory())
            continue;

        loadIDKtex( dir_entry.path().string(), srgb );
    }
}


void
idk::ModelManager::loadIDKtexpak( std::string filepath, bool srgb )
{
    idk::__texpak_file_t texpak;
    idk::filetools::texpak_load(filepath, texpak);

    for (idk::__tex_file_t &tex: texpak.texfiles)
    {
        GLuint tex_id = gltools::loadTexture(tex.w, tex.h, tex.data, srgb);
        _texture_IDs[tex.name] = tex_id;
    }
}


int
idk::ModelManager::loadOBJ( std::string raw_obj, std::string raw_mtl )
{
    _load_mtl(raw_mtl);
    int model_id = _models.create(_load_obj(raw_obj));
    idk::Model &model = _models.get(model_id);

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

    // UV
    gl::vertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(idk::Vertex), offset);
    gl::enableVertexAttribArray(2);
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
