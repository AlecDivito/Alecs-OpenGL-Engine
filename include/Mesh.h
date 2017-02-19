#ifndef MESH_H
#define MESH_H

// Std. Includes
#include <string>
#include <vector>
#include "Shader.h"
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    std::string type;
};

class Mesh
{
    public:
        /* Mesh Data */
        std::vector<Vertex> Vertices;
        std::vector<GLuint> Indices;
        std::vector<Texture> Textures;
        /* Functions */
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
        virtual ~Mesh();
        void Draw(Shader shader);
    protected:
    private:
        /* Render data */
        GLuint VAO, VBO, EBO;
        /* Functions */
        void setupMesh();
};

#endif // MESH_H
