#version 330 core

// fragment shader requires one vec4 output
out vec4 color;

in vec3 ourColors;

void main()
{
    color = vec4(ourColors, 1.0);
}
