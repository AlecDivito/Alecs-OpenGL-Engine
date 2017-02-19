#version 330 core

// fragment shader requires one vec4 output
out vec4 color;

in vec3 ourColors;
in vec2 TexCoord;

uniform sampler2D ourTexture; // this will be used to send the image

void main()
{
    color = texture(ourTexture, TexCoord);
}
