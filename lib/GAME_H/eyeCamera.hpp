#ifndef EYECAMERA_H
#define EYECAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "camera.h"

constexpr float SENSITIVITY = 0.1f;
constexpr float MAXPITCHANGLE = 45.0f;
constexpr float MINYAWANGLE = 45.0f;
constexpr float MAXYAWANGLE = 135.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class eyeCamera : public Camera {

public:
    // camera Attributes
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    float MinYawAngle;
    float MaxYawAngle;
    // camera options
    float Zoom;

    // constructor with vectors
    eyeCamera(glm::vec3 position = glm::vec3(CAMSTART[0], CAMSTART[1], CAMSTART[2]))
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Zoom(ZOOM), Yaw(YAW), Pitch(PITCH), MinYawAngle(MINYAWANGLE), MaxYawAngle(MAXYAWANGLE), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
    {
        m_cameraType = 1;
        m_position     = position;
        MouseSensitivity = SENSITIVITY;
        updateCameraVectors();
    }

    void setDirection(float degrees) 
    {
        Yaw = degrees;
        MaxYawAngle = degrees + 45.0f;
        MinYawAngle = degrees - 45.0f;
        updateCameraVectors();
    };

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix(glm::vec3 player_pos, bool down)
    {
        m_position = player_pos;
        m_position.y += 0.1;
        if(!down)
            m_position.y += 0.1;

        return glm::lookAt(m_position, m_position + Front, Up);
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (Pitch > MAXPITCHANGLE)
                Pitch = MAXPITCHANGLE;
            if (Pitch < -MAXPITCHANGLE)
                Pitch = -MAXPITCHANGLE;
            if (Yaw > MaxYawAngle)
                Yaw = MaxYawAngle;
            if (Yaw < MinYawAngle)
                Yaw = MinYawAngle; 
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 20.0f)
            Zoom = 20.0f;
        if (Zoom > 70.0f)
            Zoom = 70.0f;
    }

    // Resets all the camera values back to the defaults
    void resetEyeCamera()
    {
        m_position       = glm::vec3(CAMSTART[0], CAMSTART[1], CAMSTART[2]);
        WorldUp          = glm::vec3(0.0f, 1.0f, 0.0f);
        Yaw              = YAW;
        Pitch            = PITCH;
        Front            = glm::vec3(0.0f, 0.0f, -1.0f);
        MouseSensitivity = SENSITIVITY;
        Zoom             = ZOOM;
        updateCameraVectors();
    }

public:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front   = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif