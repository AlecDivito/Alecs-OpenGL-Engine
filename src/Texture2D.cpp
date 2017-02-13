#include "Texture2D.h"

Texture2D::Texture2D()
    :Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
    // Generating a Texture
    glGenTextures(1, &this->Texture);
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char* image)
{
    // Set texture as the current active texture object
    glBindTexture(GL_TEXTURE_2D, this->Texture);
    // Set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

    //Now that the texture is bound, we can start generating a texture using the previously loaded image data
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, image);
    // Once glTexImage2D is called, the currently bound texture object now has the texture image attached to it
    // we could call glGenerateMipmap after generating the texture. This will automatically generate all the required mipmaps for the currently bound texture.
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->Texture);
}
