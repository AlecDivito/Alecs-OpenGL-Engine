#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>

#include "GameObject.h"

class Cube : public GameObject
{
    public:
        // Cubes opengl IDs
        GLuint VBO, VAO;
        // Constructor and deconstructor
        Cube();
        Cube(Texture2D texture);
        virtual ~Cube();
        // Bind Objects
        void Bind();
        // Draw sprite
        void Draw();
        // collision
        GLboolean CheckCollision(GameObject &obj);
        // Set up our vertex data (and buffer(s)) and attribute pointers
        const static GLfloat Vertices[];
    protected:
    private:
};

#endif // CUBE_H
