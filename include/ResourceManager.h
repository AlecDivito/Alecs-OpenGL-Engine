#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>

#include <SOIL.h>

#include "Texture2D.h"
#include "Shader.h"

// Manage Textures and shader for the program
// Singleton class so the constructor is private
class ResourceManager
{
    public:
        static Texture2D addTexture(std::string path, std::string name, GLboolean alpha = false);
        static Texture2D getTexture(std::string name);

        static Shader addShader(std::string name, std::string vShaderFile,std::string fShaderFile,std::string gShaderFile = "");
        static Shader getShader(std::string name);

        static void clear();
    protected:
    private:
        ResourceManager();
        static Texture2D createTexture(std::string path, GLboolean alpha);
        static Shader createShader(std::string vShaderFile,std::string fShaderFile,std::string gShaderFile = "");

        static std::map<std::string, Texture2D> textures;
        static std::map<std::string, Shader> shaders;
};

#endif // RESOURCEMANAGER_H
