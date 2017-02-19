#include "Light.h"
#include "Cube.h"

Light::Light()
{
    // store vertices in a buffer to manage them on the graphics card
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*288, Cube::Vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // TexCoords attibute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

Light::~Light()
{
    //dtor
}

// Bind Objects
void Light::Bind()
{
    glBindVertexArray(VAO);
}

// Draw sprite
void Light::Draw()
{
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// collision
GLboolean Light::CheckCollision(GameObject &obj)
{
    return true;
}

