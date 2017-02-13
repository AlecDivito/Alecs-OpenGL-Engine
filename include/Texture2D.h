#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <GL/glew.h>

class Texture2D
{
    public:
        // Holds the texture ID
        GLuint Texture;
        // Texture formats
        GLuint Internal_Format;
        GLuint Image_Format;
        // Texture configuration
        GLuint Wrap_S; // Wrapping mode on S axis
        GLuint Wrap_T; // Wrapping mode on T axis
        GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
        GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels
        // Constructor (sets default texture modes
        Texture2D();
        // Generates texture from image data
        void Generate(GLuint width, GLuint height, unsigned char* data);
        // Binds the texture as the current active GL_TEXTURE_2D texture object
        void Bind() const;
    protected:
    private:
};

#endif // TEXTURE2D_H
