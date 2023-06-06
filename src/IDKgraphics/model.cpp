#include "model.h"
#include "model.h"


static void charreplace(std::string &str, char from, char to)
{
    for (char &c: str)
        if (c == from)
            c = to;
}

static void charreplace(idk::vector<std::string> &strs, char from, char to)
{
    for (std::string &str: strs)
        charreplace(str, from, to);
}


void
idk::Model::_load_obj(std::string path)
{
    std::ifstream instream(path);
    std::string line;

    idk::vector<glm::vec4> positions, normals;
    idk::vector<glm::vec2> uvs;

    while (getline(instream, line))
    {
        std::istringstream iss(line);
        std::string dummy;

        if (line.find("v ") != std::string::npos)
        {
            float x, y, z;
            iss >> dummy >> x >> y >> z;
            positions.push(glm::vec4(x, y, z, 1.0f));
        }

        else if (line.find("vn ") != std::string::npos)
        {
            float x, y, z;
            iss >> dummy >> x >> y >> z;
            normals.push(glm::vec4(x, y, z, 0.0f));
        }

        else if (line.find("vt ") != std::string::npos)
        {
            float u, v;
            iss >> dummy >> u >> v;
            uvs.push(glm::vec2(u, v)); 
        }


        else if (line.find("usemtl ") != std::string::npos)
        {
            meshes.push(Mesh());
            IBOS.push(0);
        }

        else if (line.find("f ") != std::string::npos)
        {
            idk::vector<std::string> vstrs(3);
            iss = std::istringstream(line);
            iss >> dummy >> vstrs[0] >> vstrs[1] >> vstrs[2];

            charreplace(vstrs, '/', ' ');

            for (size_t i=0; i<3; i++)
            {
                meshes.back().vertex_indices.push(_vertices.size());
                
                iss = std::istringstream(vstrs[i]);
                _vertices.push(idk::vertex());
                
                size_t pos, norm, uv;
                iss >> pos >> norm >> uv;
                pos -= 1; norm -= 1; uv -= 1;
                _vertices.back().position  = positions[pos];
                _vertices.back().normal    = normals[norm];
                // _vertices.back().tangent = tangent(norm);
                _vertices.back().texcoords = uvs[uv];
            }
        }
    }
}


void
idk::Model::_load_mtl(std::string path)
{
    std::ifstream instream(path);
    std::string line;

    while(getline(instream, line))
    {
        if (line.find("newmtl") != std::string::npos)
        {
            
        }
    }

}



idk::Model::Model(std::string root, std::string obj, std::string mtl)
{
    _load_obj(root + obj);
    _load_mtl(root + mtl);

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
    GLCALL( glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(idk::vertex), (void *)offset); )
    GLCALL( offset += 4 * sizeof(float); )
    GLCALL( glEnableVertexAttribArray(0); )

    // Normal
    GLCALL( glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(idk::vertex), (void *)offset); )
    GLCALL( offset += 4 * sizeof(float); )
    GLCALL( glEnableVertexAttribArray(1); )

    // UV
    GLCALL( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(idk::vertex), (void *)offset); )
    GLCALL( offset += 2 * sizeof(float); )
    GLCALL( glEnableVertexAttribArray(2); )


    // Indexing
    for (size_t i=0; i<IBOS.size(); i++)
    {
        GLCALL( glDeleteBuffers(1, &IBOS[i]); )
        GLCALL( glGenBuffers(1, &IBOS[i]); )
        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOS[i]); )
        GLCALL(
            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                mesh.vertex_indices.size() * sizeof(GLuint),
                &mesh.vertex_indices[0],
                GL_STATIC_DRAW
            );
        )
    }
}
