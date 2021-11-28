#ifndef PLAYER_H
#define PLAYER_H
#include <glm/glm.hpp>
#include "element.hpp"
#include <vector>
#include "camera.h"

//constexpr glm::vec3 PLAYERSTART(1.0f, 0.0f, 1.0f);
//constexpr float PLAYERJUMPHEIGHT = PLAYERSTART.y + 0.4f;

class Player
{
private:
    glm::vec3 m_pos;
    Camera *m_camera;
    Shader m_shader;
    unsigned int m_texture;
    unsigned int m_VAO, m_VBO;

public:
    bool onGround;
    bool isFalling;
    Player(Camera *camera, glm::vec3 pos = glm::vec3(1.0f, 0.0f, 1.0f)) : m_camera(camera), m_pos(pos) 
    {
        onGround = true;
        isFalling = false;
    };
    Camera* getCamera() const {return m_camera;};
    void setCamera(Camera* camera) {m_camera = camera;};
    glm::vec3 getPos() const {return m_pos;};
    void initCube();
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model) const;
/*
    // camera Attributes
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(m_pos, m_pos + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        bool  positionChanged = true;
        int   PosX            = m_pos.x;
        int   PosY            = m_pos.y;
        int   PosZ            = m_pos.z;
        if (direction == LEFT)
            m_pos -= Right * velocity; //on veut se déplacer d'une seule case
        if (direction == RIGHT)
            m_pos += Right * velocity;
        if (onGround) {
            // feet on the floor
            m_pos.y = PLAYERSTART.y;
        }
        else if (!onGround && !isFalling) {
            //rising
        }
        else {
            //falling
        }

        // check collision
        if (m_pos.x > 5.f || m_pos.z > 5.f ||
            m_pos.x < -5.f || m_pos.z < -5.f) {
            if (direction == FORWARD) {
                m_pos -= Front * velocity;
            }
            if (direction == BACKWARD) {
                m_pos += Front * velocity;
            }
            if (direction == LEFT) {
                m_pos += Right * velocity;
            }
            if (direction == RIGHT) {
                m_pos -= Right * velocity;
            }
            positionChanged = false;
        }
        updateCameraVectors();
    }

    void Jump()
    {
        if (onGround) {
            isFalling = false;
            onGround  = false;
            //      updateCameraVectors();
        }
    }
    void Rise(float deltatime)
    {
        if (m_pos.y <= PLAYERJUMPHEIGHT) {
            m_pos.y += (1.5f * deltatime);
        }
        else {
            isFalling = true;
        }
    }
    void Fall(float deltatime)
    {
        if (m_pos.y <= PLAYERSTART.y + 0.05f) {
            m_pos.y = PLAYERSTART.y;
            onGround   = true;
        }
        else {
            m_pos.y -= (1.5f * deltatime);
        }
        updateCameraVectors();
    }

private:
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
    }*/

};

#endif