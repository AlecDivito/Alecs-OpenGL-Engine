#version 330 core

// fragment shader requires one vec4 output
out vec4 color;

in vec4 ourColors;

void main()
{
    color = ourColors;
}
