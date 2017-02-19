#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 transform;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    gl_Position = transform * vec4(position, 1.0f);
    // By doing this FragPos is in the world space
    FragPos = vec3(model * vec4(position, 1.0f));
    // doing this makes the light have a softer appearence on the objects
    // FINDING THE inverse of the
    Normal = mat3(transpose(inverse(model))) * normal;
    // NOTE:
    /*
     * Inversing matrices is a costly operation even for shaders so wherever
     * possible, try to avoid doing inverse operations in shaders since they have
     * to be done on each vertex of your scene. For learning purposes this is fine,
     * but for an efficient application you'll likely want to calculate the normal
     * matrix on the CPU and send it to the shaders via a uniform before drawing
     * (just like the model matrix).
     */
    TexCoords = texCoords;
}
