#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 normals;
layout (location = 2) in vec2 texCoord;

out vec3 ourColors;
out vec2 TexCoord;

uniform mat4 transform;
uniform vec4 colors;

void main()
{
    // At the end of the main function, whatever we set gl_Position to will be used as the output of the vertex shader
    gl_Position = transform * vec4(position, 1.0);
    ourColors = vec3(colors.xyz);
    TexCoord = texCoord;
}
