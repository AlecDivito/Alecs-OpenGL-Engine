#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colors;

out vec3 ourColors;

uniform mat4 transform;

void main()
{
    // At the end of the main function, whatever we set gl_Position to will be used as the output of the vertex shader
    gl_Position = transform * vec4(position, 1.0);
    ourColors = colors;
}
