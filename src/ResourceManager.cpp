#include "ResourceManager.h"

ResourceManager::ResourceManager() { }

std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Shader>    ResourceManager::shaders;

Texture2D ResourceManager::addTexture(std::string path, std::string name, GLboolean alpha)
{
    textures[name] = createTexture(path, alpha);
    return textures[name];
}

Texture2D ResourceManager::getTexture(std::string name)
{
    return textures[name];
}

Shader ResourceManager::addShader(std::string name, std::string vShaderFile,std::string fShaderFile,std::string gShaderFile)
{
    shaders[name] = createShader(vShaderFile, fShaderFile, gShaderFile);
    return shaders[name];
}

Shader ResourceManager::getShader(std::string name)
{
    return shaders[name];
}

void ResourceManager::clear()
{
    textures.clear();
    shaders.clear();
}

Texture2D ResourceManager::createTexture(std::string path, GLboolean alpha)
{
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // Load and generate the texture
    int width, height;
    // load the image using SOIL
    unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    // Now generate the texture
    texture.Generate(width, height, image);
    // Finally free image data
    SOIL_free_image_data(image);

    return texture;
}

Shader ResourceManager::createShader(std::string vShaderFile,std::string fShaderFile,std::string gShaderFile)
{
    Shader shader(vShaderFile, fShaderFile, gShaderFile);
    return shader;
}

