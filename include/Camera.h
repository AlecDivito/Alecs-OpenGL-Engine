#ifndef CAMERA_H
#define CAMERA_H


#include <vector>
// GLEW
#include <GL/glew.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  5.0f;
const GLfloat SENSITIVTY =  0.25f;

class Camera
{
    public:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        // Eular Angles
        GLfloat Yaw;
        GLfloat Pitch;
        // camera options
        GLfloat MovementSpeed;
        GLfloat MouseSensitivity;
        // Contructor
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f),
               GLfloat yaw = YAW, GLfloat pitch = PITCH);
        Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX,
               GLfloat upY,  GLfloat upZ,  GLfloat yaw,  GLfloat pitch);

        // Get the view matrix
        glm::mat4 GetViewMatrix();

        // Processes input received from any keyboard-like input system.
        void ProcessKeyboard(Camera_Movement, GLfloat deltaTime);

        // Processes input received from a mouse input system.
        void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    protected:
    private:
        // Calculates the front vector from the Camera's (update) Eular Angles
        void updateCameraVectors();
};

#endif // CAMERA_H
