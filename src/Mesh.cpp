#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
{
    this->Vertices = vertices;
    this->Indices = indices;
    this->Textures = textures;

    this->setupMesh();
}

Mesh::~Mesh()
{
    //dtor
}

void Mesh::Draw(Shader shader)
{
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for(GLuint i = 0; i < this->Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Acivate proper texture unit before binding
        // Retrieve texture number (the N in diffuse_textureN)
        std::string name = this->Textures[i].type;
        std::string number = (name == "texture_diffuse")
            ? std::to_string(diffuseNr++)
            : std::to_string(specularNr++);

        shader.SetFloat(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, this->Textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // Draw mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

// Structs have a great property in C++ that their memory layout is sequential. That is if we
// were to represent a struct as an array of data it would only contain the struct's variables
// in sequential order which directly translates to a float (actually byte) array that we want
// for an array buffer
// example:
    /*  Vertex vertex;
        vertex.Position = glm::vec3(0.2f, 0.4f, 0.6f);
        vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        vertex.TexCoords = glm::vec2(1.0f, 0.0f);
        // = [0.2f, 0.4f, 0.6f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f]; */

    glBufferData(GL_ARRAY_BUFFER, this->Vertices.size() * sizeof(Vertex),
                 &this->Vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->Indices.size() * sizeof(GLuint),
                 &this->Indices[0], GL_STATIC_DRAW);

// Another great use of structs is a preprocessor directive called offsetof(s,m) that takes as
// its first argument a struct and as its second argument a variable name of the struct. The
// macro returns the byte offset of that variable from the start of the struct.

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (GLvoid*)offsetof(Vertex, Normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (GLvoid*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

