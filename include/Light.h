#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>

#include <GameObject.h>


class Light : public GameObject
{
    public:
        Light();
        virtual ~Light();
        // hold the vertex array and vertex buffer
        GLuint VAO, VBO;
        // Bind Objects
        void Bind();
        // Draw sprite
        void Draw();
        // collision
        GLboolean CheckCollision(GameObject &obj);

    protected:

    private:
};

#endif // LIGHT_H
