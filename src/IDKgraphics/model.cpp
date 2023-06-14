#include "model.h"
#include "model.h"


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
idk::Model::_load_obj(std::string path)
{
    std::ifstream instream(path);
    std::string line;

    std::vector<glm::vec3> positions, normals;
    std::vector<glm::vec2> uvs;

    while (getline(instream, line))
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
            uvs.push_back(glm::vec2(u, 1.0f - v)); 
        }

        else if (line.find("usemtl") != std::string::npos)
        {
            meshes.push_back(Mesh());
            IBOS.push_back(0);

            std::string material_name;
            iss >> dummy >> material_name;
            meshes.back().material_id = _material_ids[material_name];
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

                idk::vertex vert;
                vert.position  = positions[pos - 1];
                vert.texcoords = uvs[uv - 1];
                vert.normal    = normals[norm - 1];

                meshes.back().vertex_indices.push_back(_vertices.size());
                _vertices.push_back(vert);
            }
        }
    }
}


void
idk::Model::_load_mtl(std::string path, Allocator<Material> &materials, std::unordered_map<std::string, GLuint> &textures)
{
    std::ifstream instream(path);
    std::string line;
    uint material_id;

    while (getline(instream, line))
    {
        std::istringstream iss(line);
        std::string dummy;

        if (line.find("newmtl") != std::string::npos)
        {
            material_id = materials.add();

            std::string material_name;
            iss >> dummy >> material_name;

            _material_ids[material_name] = material_id;

            materials.get(material_id).albedo_texture = textures["assets/textures/default-diffuse"];
            materials.get(material_id).specular_texture = textures["assets/textures/default-lightmap"];
        }

        else if (line[0] == 'N' && line[1] == 's')
        {
            float Ns;
            iss >> dummy >> Ns;
            materials.get(material_id).specular_exponent = Ns;
        }

        else if (line[0] == 'K' && line[1] == 's')
        {
            float x, y, z;
            iss >> x >> y >> z;
            materials.get(material_id).specular_color = glm::vec3(x, y, z);
        }

        if (line.find("map_Kd") != std::string::npos)
        {
            size_t root = line.find("assets/");
            line = line.substr(root);
            Material &material = materials.get(material_id);
            material.albedo_texture = textures[line];
        }

        if (line.find("map_Ks") != std::string::npos)
        {
            size_t root = line.find("assets/");
            line = line.substr(root);
            Material &material = materials.get(material_id);
            material.specular_texture = textures[line];
        }
    }
}


idk::Model::Model(std::string root, std::string obj, std::string mtl, Allocator<Material> &materials, std::unordered_map<std::string, GLuint> &textures)
{
    _load_mtl(root + mtl, materials, textures);
    _load_obj(root + obj);

    GLCALL( glDeleteBuffers(IBOS.size(), &IBOS[0]); )
    GLCALL( glGenBuffers(IBOS.size(), &IBOS[0]); )
    for (size_t i=0; i<meshes.size(); i++)
        _gen_mesh_IBO(i);
}


void
idk::Model::_gen_mesh_IBO(size_t mesh_idx)
{
    idk::Mesh &mesh = meshes[mesh_idx];
    
    GLCALL( glDeleteVertexArrays(1, &mesh.VAO); )
    GLCALL( glDeleteBuffers(1, &mesh.VBO); )
    GLCALL( glGenVertexArrays(1, &mesh.VAO); )
    GLCALL( glGenBuffers(1, &mesh.VBO); )

    GLCALL( glBindVertexArray(mesh.VAO); )
    GLCALL( glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO); )

    GLCALL(
        glBufferData(
            GL_ARRAY_BUFFER,
            _vertices.size() * sizeof(idk::vertex),
            &_vertices[0],
            GL_STATIC_DRAW
        );
    )

    unsigned long offset = 0;

    // Position
    GLCALL( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(idk::vertex), (void *)offset); )
    GLCALL( glEnableVertexAttribArray(0); )
    offset += 3 * sizeof(float);

    // Normal
    GLCALL( glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(idk::vertex), (void *)offset); )
    GLCALL( glEnableVertexAttribArray(1); )
    offset += 3 * sizeof(float);

    // UV
    GLCALL( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(idk::vertex), (void *)offset); )
    GLCALL( glEnableVertexAttribArray(2); )
    offset += 2 * sizeof(float);


    // Indexing
    GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOS[mesh_idx]); )
    GLCALL(
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            mesh.vertex_indices.size() * sizeof(GLuint),
            &mesh.vertex_indices[0],
            GL_STATIC_DRAW
        );
    )

    GLCALL( glBindBuffer(GL_ARRAY_BUFFER, 0); )
    GLCALL( glBindVertexArray(0); )
}
