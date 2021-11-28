#ifndef TRACKBALLCAMERA_H
#define TRACKBALLCAMERA_H
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "camera.h"
#include "cube.hpp"




// Default camera values
const float TRACKSENSITIVITY =  0.8f;
const float TRACKZOOM        =  5;


class TrackballCamera : public Camera
{
public:
    // camera Attributes
    float m_fAngleX;
    float m_fAngleY;
    float m_fDistance;

    // camera options
    float m_mouseSensitivity;
    float MovementSpeed;

// camera Attributes
    //glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

        // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        m_fDistance -= (float)yoffset;
        if (m_fDistance < 1.0f)
            m_fDistance = 1.0f;
        if (m_fDistance > 10.0f)
            m_fDistance = 10.0f; 
    }
    void rotateLeft(float degrees)
    {
        m_fAngleY += degrees;
    }
    void rotateUp(float degrees)
    {
        m_fAngleX -= degrees;
    }

    TrackballCamera(float distance = TRACKZOOM, float angleX = 0, float angleY = 0) : m_fDistance(distance), m_fAngleX(angleX), m_fAngleY(angleY), m_mouseSensitivity(TRACKSENSITIVITY) {
        m_cameraType = 0;
    }

    glm::mat4 GetViewMatrix(glm::vec3 player_pos) const
    {
        glm::mat4 Translate = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -m_fDistance));
        glm::mat4 RotateX = glm::rotate(glm::mat4(1.f), glm::radians(m_fAngleX), glm::vec3(1, 0, 0));
        glm::mat4 RotateY =  glm::rotate(glm::mat4(1.f), glm::radians(m_fAngleY), glm::vec3(0, 1, 0));
        glm::mat4 LookTo = glm::translate(glm::mat4(1.f), player_pos);
        glm::mat4 ViewMatrix = glm::mat4(Translate * RotateX * RotateY * LookTo);
    
        return ViewMatrix;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= m_mouseSensitivity;
        yoffset *= m_mouseSensitivity;

        rotateLeft(xoffset);
        rotateUp(yoffset);

        // make sure that AngleX is not out of bounds
        if (m_fAngleX > 3600.0f)
            m_fAngleX = 3600.0f;
        if (m_fAngleX < -180.0f)
            m_fAngleX = -180.0f;
    }

    virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime) {};

};

#endif