#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>

#include "GameObject.h"

class Cube : public GameObject
{
    public:
        // Cubes opengl IDs
        GLuint VBO, VAO;
        Cube();
        virtual ~Cube();
        // Bind Objects
        void Bind();
        // Draw sprite
        void Draw();
        // collision
        GLboolean CheckCollision(GameObject &obj);

    protected:
    private:
    // Set up our vertex data (and buffer(s)) and attribute pointers
    const static GLfloat vertices[];
};

#endif // CUBE_H
