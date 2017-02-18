#include "Game.h"

Game::Game(GLuint width, GLuint height) :
           State(GAME_ACTIVE), Width(width), Height(height)
{
    //ctor
}

Game::~Game()
{
    ResourceManager::Clear();
}

void Game::Init()
{
    // Load shaders
    ResourceManager::LoadShader("shaders/shader.vs", "shaders/shader.frag", NULL, "shader");
    ResourceManager::LoadShader("shaders/terrain.vs", "shaders/terrain.frag", NULL, "terrain");
    // Set render-specific controls

    // Load textures
    ResourceManager::LoadTexture("textures/container.jpg", GL_TRUE, "wall");


}

void Game::Update(GLfloat dt)
{
    // Update objects
    // This is where we can updated the numbers on the rotating boxes

    // Check for collisions
    this->DoCollisions();
}


void Game::DoCollisions()
{
    // For right now there are no collision calculations
}

void Game::ProcessInput(GLfloat dt)
{
    if(this->State == GAME_ACTIVE)
    {
        // Camera controls
        if(this->Keys[GLFW_KEY_W])
            this->camera.ProcessKeyboard(FORWARD, dt);
        if(this->Keys[GLFW_KEY_S])
            this->camera.ProcessKeyboard(BACKWARD, dt);
        if(this->Keys[GLFW_KEY_A])
            this->camera.ProcessKeyboard(LEFT, dt);
        if(this->Keys[GLFW_KEY_D])
            this->camera.ProcessKeyboard(RIGHT, dt);
    }
}

void Game::ProcessMouseInput(GLfloat xoffset, GLfloat yoffset)
{
    camera.ProcessMouseMovement(xoffset,yoffset);
}

void Game::Render()
{
    if(this->State == GAME_ACTIVE)
    {
        //ResourceManager::DrawGameObjects();
        //this->cube.Draw();
    }
}
