// STL
#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Personal
#include "ResourceManager.h"
#include "Game.h"

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

    // How to draw the triangles
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while(!glfwWindowShouldClose(window)) // checks if window was instructed to close
    {
        // Calculating delta time soon
        GLfloat time = glfwGetTime();
        GLfloat deltaTime = time - lastFrame;
        lastFrame = time;
        glfwPollEvents(); // checks if any events are triggered (like keyboard input or mouse movement events) and calls the corresponding functions

        // Process keyboard inputs
        game.ProcessInput(deltaTime);

        // Update objects
        game.Update(deltaTime);

        // Render shit
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Sets the background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        game.Render();

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
