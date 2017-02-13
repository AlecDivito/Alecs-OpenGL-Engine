#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLM
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
    public:
        // State
        GLuint Program;
        Shader() {};
        Shader(std::string vertexShaderPath, std::string fragmentShaderPath, std::string geometryShaderPath="");
        void Use();
        // Shader helper functions
        void    SetFloat    (const GLchar* name, GLfloat value,          GLboolean useShader = false);
        void    SetInteger  (const GLchar* name, GLint   value,          GLboolean useShader = false);
        void    SetVector2f (const GLchar* name, GLfloat x, GLfloat y,   GLboolean useShader = false);
        void    SetVector2f (const GLchar* name, const glm::vec2 &value, GLboolean useShader = false);
        void    SetVector3f (const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
        void    SetVector3f (const GLchar* name, const glm::vec3 &value, GLboolean useShader = false);
        void    SetVector4f (const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
        void    SetVector4f (const GLchar* name, const glm::vec4 &value,  GLboolean useShader = false);
        void    SetMatrix4  (const GLchar* name, const glm::mat4 &matrix, GLboolean useShader = false);

    protected:
    private:
        void printCompileError(GLuint shader, std::string type);
        std::string readShaderFile(std::string path);
};

#endif // SHADER_H
