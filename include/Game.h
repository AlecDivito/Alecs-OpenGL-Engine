#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "ResourceManager.h"
#include "MidPointTerrain.h"


enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_LOSE,
};

// game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
    public:
        // Game State
        GameState State;
        GLboolean Keys[1024];
        GLuint Width, Height;
        // view related
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f * Width/Height, 0.1f, 500.0f);
        // Player Object (which is a camera for right now)
        Camera camera;
        // Game objects
        Cube * cube;
        MidPointTerrain * terrain;
        // Constructor / Destructor
        Game(GLuint width, GLuint height);
        virtual ~Game();
        // Initialize game state (load all shaders/textures)
        void Init();
        // GameLoop
        void DoCollisions();
        void ProcessInput(GLfloat dt);
        void ProcessMouseInput(GLfloat xoffset, GLfloat yoffset);
        void Update(GLfloat dt);
        void Render();
    protected:
    private:
};

#endif // GAME_H
