// STL
#include <cmath>
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
#include "include/Shader.h"
#include "include/Texture2D.h"

// Window dimensions
const GLuint WIDTH = 1600, HEIGHT = 900;

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

    // Set up Key Callbacks
    glfwSetKeyCallback(window, key_callback);

    Shader shader("shaders/shader.vs", "shaders/shader.frag");

    // Triangle vertices
    GLfloat vertices[] = {
        // front                // colors           // Texture coordinates
        -1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
         1.0f, -1.0f,  1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,    1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        // back                 // colors           // Texture coordinates
        -1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,    1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    };
    GLuint indices[] = { // Note we start at 0
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3,

    };
    // Triangle positions
    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };


    // we need to store vertices in a buffer so we can manage them on the graphics card
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);// (num of buffers, [] of buffers)
    glGenBuffers(1, &EBO);
    // 1. Bind vertex Array Object
    glBindVertexArray(VAO);
    // 2. Copy our vertices array in a buffer for OpenGl to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // (specifies the targen buffer object, the buffer  object's ID)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Now do the same thing just with the element buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // We then tell how opengl should interprit the vertex data
    // (layout_Location, size of vertex attribute(vec3), dataType, GL_FALSE, length of stride, offset of where the position data begins in the buffer)
    // 3. Then we set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    // 4. We unbind the vertex array object
    glBindVertexArray(0);

    /**** Initilizing TEXTURES ****/
    Texture2D texture;
    texture.Generate("textures/wall.jpg");
    /**** Initilizing TEXTURES ****/

    // How to draw the triangles
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // enable depth testing (to show depth in objects)
    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) // checks if window was instructed to close
    {
        // Calculating delta time soon
        GLfloat time = glfwGetTime();

        glfwPollEvents(); // checks if any events are triggered (like keyboard input or mouse movement events) and calls the corresponding functions

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Sets the background color
        // buffer is cleared to the buffer specified above
        // z-buffer is cleared for the next frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 5. Draw the object
        shader.Use(); // Every shader and rendering call after glUseProgram will now use this program object
        texture.Bind();
        glBindVertexArray(VAO);
        for(int i = 0; i < (sizeof(cubePositions)/sizeof(*cubePositions)); i++)
        {
            // Identity matrix
            glm::mat4 trans;
            glm::mat4 anim = glm::rotate(trans,(GLfloat)(time / 1000.0 * 1000), glm::vec3(0.0f, 1.0f, 0.0f));
            // MODEL: push cude back a bit to not touch the camera
            glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            // VIEW: reposition the camera
            glm::mat4 view = glm::lookAt(glm::vec3(0.0, -2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
            // PROJECTION: set up a perspective
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f * WIDTH/HEIGHT, 0.1f, 100.0f);
            // Result
            trans = projection * view * model * anim;

            shader.SetVector4f("ourColor", 0.0f,0.0f,(GLfloat)sin(time),0.0f);
            shader.SetMatrix4("transform", trans);

            //glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
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
}
