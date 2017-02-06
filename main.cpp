// STL
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
std::string readFile(std::string path);

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

    /**** Initilizing Program Shader Here (for right now) ****/
    // READING SHADER FILES INTO MEMORY
    std::string vertexShaderString  = readFile("shaders/shader.vs");
    std::string fragShaderString    = readFile("shaders/shader.frag");
    const GLchar* vertexShaderSource  = vertexShaderString.c_str();
    const GLchar* fragShaderSource    = fragShaderString.c_str();

    // READING SHADER FILES INTO MEMORY
    // Checking for compile errors
    GLint success;
    GLchar infoLog[512];
    GLuint vertexShader, fragShader, shaderProgram;

    vertexShader = glCreateShader(GL_VERTEX_SHADER); // we tell what shader we want
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // specify ID, # of strings were sending, reference to those strings
    glCompileShader(vertexShader); // compile the shader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // check for errors
    if (!success)
    {   // display errors if found
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success); // check for errors
    if (!success)
    {   // display errors if found
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Creating the shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success); // check for errors
    if (!success)
    {   // display errors if found
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }

    // Delete uneed shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    /**** Initilizing Program Shader Here (for right now) ****/

    // Triangle vertices
    GLfloat vertices[] = {
        -0.5f,  -0.5f, 0.0f,
         0.5f,  -0.5f, 0.0f,
         0.0f,   0.5f, 0.0f,
    };
    // we need to store vertices in a buffer so we can manage them on the graphics card
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);// (num of buffers, [] of buffers)
    // 1. Bind vertex Array Object
    glBindVertexArray(VAO);
    // 2. Copy our vertices array in a buffer for OpenGl to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // (specifies the targen buffer object, the buffer  object's ID)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // We then tell how opengl should interprit the vertex data
    // (layout_Location, size of vertex attribute(vec3), dataType, GL_FALSE, length of stride, offset of where the position data begins in the buffer)
    // 3. Then we set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 4. We unbind the vertex array object
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)) // checks if window was instructed to close
    {
        glfwPollEvents(); // checks if any events are triggered (like keyboard input or mouse movement events) and calls the corresponding functions

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Sets the background color
        glClear(GL_COLOR_BUFFER_BIT); // buffer is cleared to the buffer specified above
        // 5. Draw the object
        glUseProgram(shaderProgram); // Every shader and rendering call after glUseProgram will now use this program object
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0); // unbind object so we dont misconfigure them elsewhere

        glfwSwapBuffers(window); // swap the color buffer (a large buffer that contains color values for each pixel in GLFW's window)
    }

    glfwTerminate(); // Terminate glfw
    return 0;
}

std::string readFile(std::string path)
{
    std::string shaderCode;
    try
    {
        // Open Files
        std::ifstream shaderFile(path);
        std::stringstream shaderStream;
        if (shaderFile.is_open())
        {
            // Read file buffer contents into streams
            shaderStream << shaderFile.rdbuf();
            // Close file handler
            shaderFile.close();
            // Convert stream into string
            shaderCode = shaderStream.str();
        }
    }
    catch(std::exception e)
    {
        std:: cout << "ERROR:SHADER: Failed to read shader file" << std::endl;
    }
    return shaderCode;
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
