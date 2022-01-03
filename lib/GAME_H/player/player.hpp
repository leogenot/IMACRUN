#ifndef PLAYER_H
#define PLAYER_H
#include <glm/glm.hpp>
#include <vector>
#include "../../miniaudio.h"
#include "../player/camera.hpp"
#include "../gamemap/element.hpp"
#include "../gamemap/gamemap.hpp"
#include "../3dmodels/model.hpp"

constexpr float SPEED = 3.f;
constexpr float PLAYERSTART[3]   = {60.0f, 0.0f, 2.0f};
constexpr float PLAYERJUMPHEIGHT = PLAYERSTART[1] + 0.4f;

class Player {
private:
    glm::vec3 m_pos;
    Camera*   m_camera;
    Shader*   m_shader;
    Model*    m_objModel;

    int  m_score;
    int  m_life = 3;
    char m_username[128];

    // sound
    ma_result   result;
    ma_engine   engine;
    std::string str            = "assets/sounds/ka_chow.mp3";
    const char* c_soundKa_chow = str.c_str();
    std::string str2             = "assets/sounds/ka_chigga.mp3";
    const char* c_soundKa_chigga = str2.c_str();

public:
    bool  onGround;
    bool  isFalling;
    bool  down;
    float MovementSpeed;

    // vectors
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;

    Player() {};

    Player(Camera* camera, glm::vec3 pos = glm::vec3(PLAYERSTART[0], PLAYERSTART[1], PLAYERSTART[2]))
        : m_camera(camera), m_pos(pos), onGround(true), isFalling(false), Front(glm::vec3(0., 0., -1.)), MovementSpeed(SPEED), Yaw(YAW), Pitch(PITCH), WorldUp(0., 1., 0.), m_score(0), m_life(5)
    {
        assert (camera != NULL && "error: Player constructor : 'camera should not be NULL");
        updatePlayerVectors();
    };

    // getters and setters
    Camera* getCamera() const { return m_camera; };
    void    setCamera(Camera* camera)
    {
        m_camera = camera;
        m_camera->setDirection(Yaw);
    };
    glm::vec3 getPos() const { return m_pos; };
    void      setPos(glm::vec3 pos) { m_pos = pos; };
    Shader    getShader() const { return *m_shader; };
    Model     getObjModel() const { return *m_objModel; };
    glm::mat4 getModel() const;
    char*     getUsername() { return &m_username[0]; };
    void      setUsername(char* s) { strcpy(m_username, s); }
    int       getScore() const { return m_score; };
    void      setScore(int score) { m_score = score; }
    int       getLife() const { return m_life; };
    void      setLife(int life) { m_life = life; }
    void      addScore(int value) { m_score += value; };

    void removeLife()
    {
        m_life--;
        result = ma_engine_init(NULL, &engine);
        if (result != MA_SUCCESS) {
            printf("Failed to initialize audio engine.");
        }
        ma_engine_play_sound(&engine, c_soundKa_chigga, NULL);
    };

    void initPlayer(Shader* shader, Model* model);
    void resetPlayer();
    bool getLosingCollision(Camera_Movement direction, GameMap* gamemap); // if collide wall or fall
    void Jump();
    void Rise(float deltatime);
    void Fall(float deltatime);
    void BendDown() { down = true; };
    void GetUp() { down = false; };
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime, GameMap* gamemap);
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updatePlayerVectors();
};

#endif