#ifndef PLAYER_H
#define PLAYER_H
#include <glm/glm.hpp>
#include <vector>
#include "camera.h"
#include "element.hpp"
#include "gamemap.hpp"
#include "model.hpp"

constexpr float SPEED            = 2.f;
constexpr float PLAYERSTART[3]   = {2.0f, 0.0f, 2.0f};
constexpr float PLAYERJUMPHEIGHT = PLAYERSTART[1] + 0.4f;

class Player {
private:
    glm::vec3 m_pos;
    Camera*   m_camera;
    Shader    m_shader;
    //Model m_objModel;

    int  m_score;
    int  m_life = 3;
    char player_username[128];

public:
    bool  onGround;
    bool  isFalling;
    float MovementSpeed;

    // vectors
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;

    Player(Camera* camera, glm::vec3 pos = glm::vec3(PLAYERSTART[0], PLAYERSTART[1], PLAYERSTART[2]))
        : m_camera(camera), m_pos(pos), onGround(true), isFalling(false), Front(glm::vec3(0., 0., -1.)), MovementSpeed(SPEED), Yaw(YAW), Pitch(PITCH), WorldUp(0., 1., 0.), m_score(0), m_life(5)
    {
        updatePlayerVectors();
    };

    Camera* getCamera() const { return m_camera; };
    void    setCamera(Camera* camera)
    {
        m_camera = camera;
        m_camera->setDirection(Yaw);
    };

    glm::vec3 getPos() const { return m_pos; };
    int       getScore() const { return m_score; };
    char*     getUsername() { return &player_username[0]; };
    void      setUsername(char* s) { strcpy(player_username, s); }
    void      addScore() { m_score++; };

    int  getLife() const { return m_life; };
    void removeLife() { m_life--; };

    void initPlayer();
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, Model objModel);
    bool getCollision(Camera_Movement direction, GameMap* gamemap);
    void Jump();
    void Rise(float deltatime);
    void Fall(float deltatime);
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime, GameMap* gamemap);
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updatePlayerVectors();

    void ResetPlayer();
};

#endif