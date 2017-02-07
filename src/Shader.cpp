#include "Shader.h"

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath, std::string geometryShaderPath)
{
    std::string vertexShaderString  = readShaderFile(vertexShaderPath);
    std::string fragShaderString    = readShaderFile(fragmentShaderPath);
    const GLchar* vertexShaderSource  = vertexShaderString.c_str();
    const GLchar* fragShaderSource    = fragShaderString.c_str();

    this->Program = glCreateProgram();
    GLuint vertexShader, fragShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER); // we tell what shader we want
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // specify ID, # of strings were sending, reference to those strings
    glCompileShader(vertexShader); // compile the shader
    printCompileError(vertexShader, "VERTEX");
    glAttachShader(this->Program, vertexShader);
    glDeleteShader(vertexShader);

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragShader);
    printCompileError(fragShader, "FRAGMENT");
    glAttachShader(this->Program, fragShader);
    glDeleteShader(fragShader);

    if (geometryShaderPath != "")
    {
        std::string geometryShaderString = readShaderFile(geometryShaderPath);
        const GLchar* geometryShaderSource = geometryShaderString.c_str();
        GLuint geometryShader;
        geometryShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
        glCompileShader(geometryShader);
        printCompileError(geometryShader, "GEOMETRY");
        glAttachShader(this->Program, geometryShader);
        glDeleteShader(geometryShader);
    }
    glLinkProgram(this->Program);
    printCompileError(this->Program, "PROGRAM");
}

void Shader::printCompileError(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (type == "PROGRAM")
    {
        if (!success)
        {   // display errors if found
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
        }
    }
    else
    {
        if (!success)
        {   // display errors if found
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::"+type+"::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

}

std::string Shader::readShaderFile(std::string path)
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
