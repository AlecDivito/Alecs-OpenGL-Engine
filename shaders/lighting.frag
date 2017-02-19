#version 330 core

out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

//First we define a specular intensity value to give the specular highlight a medium-bright color so that it doesn't have too much of an impact:
const float specularStrength = 0.5f;

void main()
{
    // Lighting.frag is using PHONG LIGHTING
    // will update when we have more information and have a
    // good way of obtaining all objects normals and have
    // mesh based and box collision

    /*
        When calculating lighting we usually do not care about the magnitude of
        a vector or their position; we only care about their direction. Because
        we only care about their direction almost all the calculations are done
        with unit vectors since it simplifies most calculations (like the dot
        product). So when doing lighting calculations, make sure you always
        normalize the relevant vectors to ensure they're actual unit vectors.
        Forgetting to normalize a vector is a popular mistake.
     */

    // Ambient color
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse color
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular light :)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0);
}

