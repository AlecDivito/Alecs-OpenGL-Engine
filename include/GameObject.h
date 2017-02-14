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
        GameObject();
        virtual ~GameObject();

    protected:

    private:
};

#endif // GAMEOBJECT_H
