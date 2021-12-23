#ifndef ENEMY_H
#define ENEMY_H
#include <glm/glm.hpp>
#include <vector>
#include "model.hpp"
#include "gamemap.hpp"
#include "camera.h"

constexpr float ENEMYSPEED      = 2.f;
constexpr float ENEMYSTART[3]   = {2.0f, 0.0f, 0.0f};
constexpr float ENEMYJUMPHEIGHT = ENEMYSTART[1] + 0.4f;

class Enemy
{
private:
    glm::vec3 m_pos;
    Shader m_shader;

public:
    bool onGround;
    bool isFalling;
    float MovementSpeed;
    // vectors
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;

    Enemy(glm::vec3 pos = glm::vec3(ENEMYSTART[0], ENEMYSTART[1], ENEMYSTART[2])) 
    : m_pos(pos), onGround(true), isFalling(false), 
    Front(glm::vec3(0., 0., -1.)), MovementSpeed(ENEMYSPEED), Yaw(YAW), Pitch(PITCH), WorldUp(0., 1., 0.) 
    {
        updateEnemyVectors();
    };

    glm::vec3 getPos() const {return m_pos;};
    void initEnemy();
    void resetEnemy();
    
    void drawEnemy(glm::mat4 view, glm::mat4 projection, glm::mat4 model, Model objModel);
    
    // jump manager
    void Jump();
    void Rise(float deltatime);
    void Fall(float deltatime);
 
    // follow the player
    void Follow(GameMap *gameMap, float deltaTime);
    // calculates the front vector from the Euler Angles
    void updateEnemyVectors();
};
#endif