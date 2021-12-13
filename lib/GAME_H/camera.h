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
    RIGHT,
    ROTATELEFT,
    ROTATERIGHT
};

// Default camera values
constexpr float ZOOM        = 70.0f;
constexpr float CAMSTART[3] = {1.0f, 0.0f, 1.0f};
constexpr float YAW         = 90.0f;
constexpr float PITCH       = 0.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
protected:
    // Attributs
    unsigned int m_cameraType; // 0 : Trackball / 1 : Eyes
    glm::vec3 m_position;

    // camera options
    float MouseSensitivity;

public:

    Camera() {};

    unsigned int getCameraType() {return m_cameraType;}
    glm::vec3 getPos() const {return m_position;};
    
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    virtual glm::mat4 GetViewMatrix(glm::vec3 player_pos) = 0;

    virtual void setDirection(float degrees) = 0;
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) = 0;
    virtual void ProcessMouseScroll(float yoffset) = 0;
};
#endif