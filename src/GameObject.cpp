#include "GameObject.h"

GameObject::GameObject() : Position(glm::vec3(0.0f)), Size(glm::vec3(0.0f)),
                           Velocity(glm::vec3(0.0f)), Color(glm::vec3(1.0f)),
                           Rotation(0.0f), IsSolid(true), Destroyed(false)
{

}

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite,
                       glm::vec3 color, glm::vec3 velocity) :
                       Rotation(0.0f), IsSolid(true), Destroyed(false)
{
    this->Position = pos;
    this->Size = size;
    this->sprite = sprite;
    this->Color = color;
    this->Velocity = velocity;
}
