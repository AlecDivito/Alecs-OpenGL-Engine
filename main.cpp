// STL
#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

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

    // Set the size of the rendering window so OpenGL displays the data and coordinates with respect to the window
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    // left, bottom, right, top of the screen coordiantes (we map (-1, 1) to (0, 800) and (0, 600))
    glViewport(0, 0, width, height);
    /**** Initilizing everything important ****/

    glfwSetKeyCallback(window, key_callback);

    while(!glfwWindowShouldClose(window)) // checks if window was instructed to close
    {
        glfwPollEvents(); // checks if any events are triggered (like keyboard input or mouse movement events) and calls the corresponding functions
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
}
