#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture2D.h"

// Container object for holder all state relevant for a single
// game object entity. Each object in the game likely needs the
// miniamal of state as described within GameObject.
class GameObject
{
    public:
        // Object state
        glm::vec3 Position, Size, Velocity;
        glm::vec3 Color;
        GLfloat Rotation;
        GLboolean IsSolid;
        GLboolean Destroyed;
        // render state
        Texture2D sprite;
        // Constructor(s)
        GameObject();
        GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite,
                   glm::vec3 color = glm::vec3(1.0f),
                   glm::vec3 velocity = glm::vec3(0.0f));
        // Draw sprite
        virtual void Draw() = 0;
        // collision
        virtual Glboolean CheckCollision(GameObject &obj) = 0;

    protected:

    private:
};

#endif // GAMEOBJECT_H
