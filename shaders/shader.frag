#version 330 core

// fragment shader requires one vec4 output
out vec4 color;

uniform vec4 ourColor;

void main()
{
    color = ourColor;
}
