#ifndef EYECAMERA_H
#define EYECAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "camera.h"

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class eyeCamera : public Camera {
public:
    // camera Attributes
    glm::vec3 Position;
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
    bool  onGround;
    bool  isFalling;

    // constructor with vectors
    eyeCamera(glm::vec3 position = glm::vec3(CAMSTART[0], CAMSTART[1], CAMSTART[2]), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        m_cameraType = 1;
        Position     = position;
        WorldUp      = up;
        Yaw          = yaw;
        Pitch        = pitch;
        onGround     = true;
        isFalling    = false;
        updateCameraVectors();
    }
    // constructor with scalar values
    eyeCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        m_cameraType = 1;
        Position     = glm::vec3(posX, posY, posZ);
        WorldUp      = glm::vec3(upX, upY, upZ);
        Yaw          = yaw;
        Pitch        = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        bool  positionChanged = true;
        int   PosX            = Position.x;
        int   PosY            = Position.y;
        int   PosZ            = Position.z;
        float velocity        = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        if (onGround) {
            // feet on the floor
            Position.y = CAMSTART[1];
        }
        else if (/*!onGround && */ !isFalling) {
            //rising
        }
        else {
            //falling
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
        if (Position.y <= JUMPHEIGHT) {
            Position.y += (1.5f * deltatime);
        }
        else {
            isFalling = true;
        }
    }
    void Fall(float deltatime)
    {
        if (Position.y <= CAMSTART[1] + 0.05f) {
            Position.y = CAMSTART[1];
            onGround   = true;
        }
        else {
            Position.y -= (1.5f * deltatime);
        }
        updateCameraVectors();
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
            if (Pitch > MAXLOOKANGLE)
                Pitch = MAXLOOKANGLE;
            if (Pitch < -MAXLOOKANGLE)
                Pitch = -MAXLOOKANGLE;
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
    void Reset()
    {
        Position         = glm::vec3(CAMSTART[0], CAMSTART[1], CAMSTART[2]);
        WorldUp          = glm::vec3(0.0f, 1.0f, 0.0f);
        Yaw              = YAW;
        Pitch            = PITCH;
        Front            = glm::vec3(0.0f, 0.0f, -1.0f);
        MovementSpeed    = SPEED;
        MouseSensitivity = SENSITIVITY;
        Zoom             = ZOOM;
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
    }
};
#endif