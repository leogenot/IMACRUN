#ifndef TRACKBALLCAMERA_H
#define TRACKBALLCAMERA_H
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// Default camera values
const float TRACKSENSITIVITY =  0.8f;
const float TRACKZOOM        =  5;

class TrackballCamera
{
public:
    // camera Attributes
    float m_fDistance;
    float m_fAngleX;
    float m_fAngleY;

    // camera options
    float m_mouseSensitivity;

public:
    void moveFront(float delta)
    {
        m_fDistance -= delta;
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
        m_fAngleX += degrees;
    }

    TrackballCamera(float distance = TRACKZOOM, float angleX = 0, float angleY = 0) : m_fDistance(distance), m_fAngleX(angleX), m_fAngleY(angleY), m_mouseSensitivity(TRACKSENSITIVITY) {}

    glm::mat4 GetViewMatrix() const
    {
        glm::mat4 Translate = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -m_fDistance));
        glm::mat4 RotateX = glm::rotate(glm::mat4(1.f), glm::radians(m_fAngleX), glm::vec3(1, 0, 0));
        glm::mat4 RotateY =  glm::rotate(glm::mat4(1.f), glm::radians(m_fAngleY), glm::vec3(0, 1, 0));
        glm::mat4 ViewMatrix = glm::mat4(Translate * RotateX * RotateY);
        return ViewMatrix;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset)
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
};

#endif