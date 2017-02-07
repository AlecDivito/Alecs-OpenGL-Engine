#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

class Shader
{
    public:

        GLuint Program;

        Shader(std::string vertexShaderPath, std::string fragmentShaderPath, std::string geometryShaderPath="");


    protected:
    private:
        void printCompileError(GLuint shader, std::string type);
        std::string readShaderFile(std::string path);
};

#endif // SHADER_H
