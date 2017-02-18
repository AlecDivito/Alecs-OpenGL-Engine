// STL
#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// SOIL
#include <SOIL.h>
// Personal
#include "ResourceManager.h"
#include "Game.h"
#include "MidPointTerrain.h"

// Window dimensions
const GLuint WIDTH = 1600, HEIGHT = 900;

// keys
bool firstMouse = true;
GLfloat lastFrame = 0.0f;
GLfloat lastX =  WIDTH  / 2.0;
GLfloat lastY =  HEIGHT / 2.0;

/**** Initilizing Camera ****/
Game game(WIDTH, HEIGHT);
/**** Initilizing Camera ****/

// functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main()
{
    /**** Initilizing everything important ****/
        if (!glfwInit())
        {
            std::cout << "Failed to Initilize GLFW" << std::endl;
            glfwTerminate();
            return 1;
        }
        // Window hints to use
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        // Tells GLFW it needs to use opengl 3.3 and up (not allowed to use older functions)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // Tells if window is resizable
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // Initializes the window
        GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MineCraftSandbox", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return 1;
        }
        glfwMakeContextCurrent(window);

        // Initializing GLEW (Must be done this way to take a modern approch to initializing)
        glewExperimental = GL_TRUE; // (if we didn't do this, could cause issues when using the core profile)
        if (glewInit() != GLEW_OK)
        {
            std::cout << "Failed to initialize GLEW" << std::endl;
            return 1;
        }

        // Set Options
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Set the size of the rendering window so OpenGL displays the data and coordinates with respect to the window
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        // left, bottom, right, top of the screen coordiantes (we map (-1, 1) to (0, 800) and (0, 600))
        glViewport(0, 0, width, height);
        // enable depth testing (to show depth in objects)
        glEnable(GL_DEPTH_TEST);
    /**** Initilizing everything important ****/

    // Set up Key Callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    game.Init();
    MidPointTerrain terrain(ResourceManager::GetShader("terrain"), 7);
    terrain.MidPointDisplacement();
    //terrain.InitTerrainMap();
    GLuint VAO, VBO;
    std::vector<GLfloat> terrainArrayMap;
    float span = (float)terrain.Resolution;
    for(unsigned int x = 0; x < terrain.Resolution-1; ++x)
    {
        int tempX = x + 1;
        GLfloat xCoord_zero = 2 * (x/span) - 1;// x = 0
        GLfloat xCoord_one =  2 * (tempX/span) - 1;// x = 1
        for(unsigned int y = 0; y < terrain.Resolution-1; ++y)
        {
            int tempY = y + 1;
            GLfloat yCoord_zero = 2 * (y/span) - 1;// y = 0
            GLfloat yCoord_one = 2 * (tempY/span) - 1;// y = 1
            // triangle 1 (0, 0, z)
            terrainArrayMap.push_back(xCoord_zero);
            terrainArrayMap.push_back(yCoord_zero);
            terrainArrayMap.push_back((GLfloat)terrain.getHightMapValue(x, y)); // z value
            // triangle 1 (0, 1, z)
            terrainArrayMap.push_back(xCoord_zero);
            terrainArrayMap.push_back(yCoord_one);
            terrainArrayMap.push_back((GLfloat)terrain.getHightMapValue(x, tempY)); // z value
            // triangle 1 (1,0, z)
            terrainArrayMap.push_back(xCoord_one);
            terrainArrayMap.push_back(yCoord_zero);
            terrainArrayMap.push_back((GLfloat)terrain.getHightMapValue(tempX, y)); // z value
            // triangle 2 (0, 1, z)
            terrainArrayMap.push_back(xCoord_zero);
            terrainArrayMap.push_back(yCoord_one);
            terrainArrayMap.push_back((GLfloat)terrain.getHightMapValue(x, tempY)); // z value
            // triangle 2 (1, 0, z)
            terrainArrayMap.push_back(xCoord_one);
            terrainArrayMap.push_back(yCoord_zero);
            terrainArrayMap.push_back((GLfloat)terrain.getHightMapValue(tempX, y)); // z value
            // triangle 2 (1, 1, z)
            terrainArrayMap.push_back(xCoord_one);
            terrainArrayMap.push_back(yCoord_one);
            terrainArrayMap.push_back((GLfloat)terrain.getHightMapValue(tempX, tempY)); // z value

        }
    }
 GLfloat terrainArrayMapx[(terrain.Resolution-1) * (terrain.Resolution-1) * 18];
     for (int i = 0; i < terrainArrayMap.size(); i++)
    {
        terrainArrayMapx[i] = terrainArrayMap[i];
    }
    //GLfloat * terrainArrayMapx = &terrainArrayMap[0];
    // store vertices in a buffer to manage them on the graphics card
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 1. Bind vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for opengl to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(terrainArrayMapx), terrainArrayMapx, GL_DYNAMIC_DRAW);
    // We then tell how opengl should interprit the vertex data
    // (layout_Location, size of vertex attribute(vec3), dataType, GL_FALSE, length of stride, offset of where the position data begins in the buffer)
    // 3. Then we set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 4. We unBind the vertex array object
    glBindVertexArray(0);



    // Triangle positions
    glm::vec3 cubePositions[] = {
      glm::vec3( 10.0f,  0.0f,  0.0f),
      glm::vec3( 10.0f, 10.0f, -10.0f),
      glm::vec3(-10.5f, -10.2f, -10.5f),
      glm::vec3(-10.8f, 10.0f, 10.3f),
    };

    // We create a cube
    Cube cube(ResourceManager::GetTexture("wall"));


    // How to draw the triangles
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while(!glfwWindowShouldClose(window)) // checks if window was instructed to close
    {
        // Calculating delta time soon
        GLfloat time = glfwGetTime();
        GLfloat deltaTime = time - lastFrame;
        lastFrame = time;
        glfwPollEvents(); // checks if any events are triggered (like keyboard input or mouse movement events) and calls the corresponding functions
// draw cube
        // Process keyboard inputs
        game.ProcessInput(deltaTime);

        // Update objects
        game.Update(deltaTime);

        cube.Bind(); // we bind the cube to the current context

        // Render shit
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Sets the background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        game.Render();
        ResourceManager::GetShader("shader").Use();

        // VIEW: reposition the camera
        // PROJECTION: set up a perspective
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f * WIDTH/HEIGHT, 0.1f, 500.0f);
        for(unsigned int i = 0; i < (sizeof(cubePositions)/sizeof(*cubePositions)); i++)
        {
            // Identity matrix
            glm::mat4 trans;
            glm::mat4 anim = glm::rotate(trans,(GLfloat)(time / 1000.0 * 1000), glm::vec3(0.0f, 1.0f, 0.0f));
            // MODEL: push cude back a bit to not touch the camera
            glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            // Result
            trans = projection * game.camera.GetViewMatrix() * model * anim;

            ResourceManager::GetShader("shader").SetVector4f("ourColor", 0.0f,(GLfloat)cos(time), (GLfloat)sin(time),1.0f);
            ResourceManager::GetShader("shader").SetMatrix4("transform", trans);

            cube.Draw();
        }
//end
        // Bind the array
        glBindVertexArray(VAO);

        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0,0,0));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model,  glm::vec3(15.0f));

        terrain.TerrainShader.Use();
        glm::mat4 trans = projection * game.camera.GetViewMatrix() * model;
        terrain.TerrainShader.SetMatrix4("terrainTransform", trans);
        glDrawArrays(GL_TRIANGLES, 0, terrainArrayMap.size() / 3);

        glBindVertexArray(0); // unbind object so we dont misconfigure them elsewhere

        glfwSwapBuffers(window); // swap the color buffer (a large buffer that contains color values for each pixel in GLFW's window)
    }
    glfwTerminate(); // Terminate glfw
    return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the window should close property to true,
    // Closing the applicaiton
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if(key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            game.Keys[key] = true;
        }
        else if(action == GLFW_RELEASE)
        {
            game.Keys[key] = false;
        }
    }
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;// Reversed since y-coordinates go from bottom to left


    lastX = xpos;
    lastY = ypos;
    game.camera.ProcessMouseMovement(xoffset,yoffset);
}
