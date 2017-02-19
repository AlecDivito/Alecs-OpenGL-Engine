#include "Game.h"
#include "Light.h"

    // Triangle positions
glm::vec3 cubePositions[] = {
    glm::vec3( 10.0f,  0.0f,  0.0f),
    glm::vec3( 10.0f, 10.0f, -10.0f),
    glm::vec3(-10.5f, -10.2f, -10.5f),
    glm::vec3(-10.8f, 10.0f, 10.3f),
};

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
};

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
    ResourceManager::LoadShader("shaders/lighting.vs", "shaders/lighting.frag", NULL, "light");
    ResourceManager::LoadShader("shaders/lamp.vs", "shaders/lamp.frag", NULL, "lamp");
    // Set render-specific controls

    // Load textures
    ResourceManager::LoadTexture("textures/container.jpg", GL_TRUE, "wall");

    // Load objects
    static Cube tempCube(ResourceManager::GetTexture("wall"));
    this->cube = &tempCube;
    static MidPointTerrain tempTerrain(ResourceManager::GetShader("terrain"), 5);
    tempTerrain.MidPointDisplacement();
    tempTerrain.InitTerrainMap();
    this->terrain = &tempTerrain;
    static Light light;
    this->light = &light;
}

void Game::Update(GLfloat dt)
{
    // Update objects
    // This is where we can updated the numbers on the rotating boxes
//glm::mat4 anim = glm::rotate(trans,(GLfloat)(time / 1000.0 * 1000), glm::vec3(0.0f, 1.0f, 0.0f));

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
        if(this->Keys[GLFW_KEY_F5]) {
            this->terrain->MidPointDisplacement();
            this->terrain->InitTerrainMap();
        }
        if(this->Keys[GLFW_KEY_X])
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if(this->Keys[GLFW_KEY_F])
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
        // Draw cubes
        ResourceManager::GetShader("light").Use();
        // viewPos
        ResourceManager::GetShader("light").SetVector3f("viewPos", camera.Position);
        // Directional light
        ResourceManager::GetShader("light").SetVector3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ResourceManager::GetShader("light").SetVector3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        ResourceManager::GetShader("light").SetVector3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        ResourceManager::GetShader("light").SetVector3f("dirLight.specular", 0.5f, 0.5f, 0.5f);
            // Point light 1
        ResourceManager::GetShader("light").SetVector3f("pointLights[0].position", pointLightPositions[0]);
        ResourceManager::GetShader("light").SetVector3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        ResourceManager::GetShader("light").SetVector3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        ResourceManager::GetShader("light").SetVector3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        ResourceManager::GetShader("light").SetFloat("pointLights[0].constant", 1.0f);
        ResourceManager::GetShader("light").SetFloat("pointLights[0].linear", 0.09);
        ResourceManager::GetShader("light").SetFloat("pointLights[0].quadratic", 0.032);
            // Spot light
        ResourceManager::GetShader("light").SetVector3f("spotLight.position", camera.Position);
        ResourceManager::GetShader("light").SetVector3f("spotLight.direction", camera.Front);
        ResourceManager::GetShader("light").SetVector3f("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        ResourceManager::GetShader("light").SetVector3f("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        ResourceManager::GetShader("light").SetVector3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
        ResourceManager::GetShader("light").SetFloat("spotLight.constant", 1.0f);
        ResourceManager::GetShader("light").SetFloat("spotLight.linear", 0.09);
        ResourceManager::GetShader("light").SetFloat("spotLight.quadratic", 0.032);
        ResourceManager::GetShader("light").SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        ResourceManager::GetShader("light").SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        for(unsigned int i = 0; i < (sizeof(cubePositions)/sizeof(*cubePositions)); i++)
        {
            // render cubes
            this->cube->Bind();
            glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            glm::mat4 trans = this->projection * camera.GetViewMatrix() * model;
            // Object
            ResourceManager::GetShader("light").SetMatrix4("transform", trans);
            ResourceManager::GetShader("light").SetMatrix4("model", model);
            this->cube->Draw();
        }

        // Light cubes
        ResourceManager::GetShader("lamp").Use();
        this->light->Bind();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pointLightPositions[0]));
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        glm::mat4 trans = this->projection * camera.GetViewMatrix() * model;
        ResourceManager::GetShader("lamp").SetMatrix4("transform", trans);
        this->light->Draw();

        // draw terrain
        ResourceManager::GetShader("terrain").Use();
        this->terrain->Bind();
        glm::mat4 terrainModel;
        terrainModel = glm::translate(terrainModel, pointLightPositions[0]);
        terrainModel = glm::rotate(terrainModel, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        terrainModel = glm::scale(terrainModel,  glm::vec3(3.0f,3.0f,3.5));
        glm::mat4 terrainTrans = projection * camera.GetViewMatrix() * terrainModel;
        ResourceManager::GetShader("terrain").SetMatrix4("transform", terrainTrans);
        terrain->Draw();
    }
}
