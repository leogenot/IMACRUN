#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
constexpr float YAW         = -90.0f;
constexpr float PITCH       = 0.0f;
constexpr float SPEED       = 1.2f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM        = 70.0f;
constexpr float CAMSTART[3] = {5.0f, 0.0f, 3.0f};
constexpr float JUMPHEIGHT = CAMSTART[1] + 0.4f;
constexpr float MAXLOOKANGLE = 35.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
protected:
    // Attributs
    unsigned int m_cameraType; // 0 : Trackball / 1 : Eyes

public:
    //Methods
    unsigned int getCameraType()
    {
        return m_cameraType;
    }
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    virtual glm::mat4 GetViewMatrix()const = 0;

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime) = 0;

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) = 0;

    virtual void ProcessMouseScroll(float yoffset)=0;

};
#endif