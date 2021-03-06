#ifndef ENEMY_H
#define ENEMY_H
#include <glm/glm.hpp>
#include <vector>
#include "../3dmodels/model.hpp"
#include "../gamemap/gamemap.hpp"
#include "../player/camera.hpp"

constexpr float ENEMYSPEED      = 2.5f;
constexpr float ENEMYSTART[3]   = {60.0f, 0.0f, 0.0f};
constexpr float ENEMYJUMPHEIGHT = ENEMYSTART[1] + 0.6f;

class Enemy
{
private:
    glm::vec3 m_pos;
    Shader*   m_shader;
    Model*    m_objModel;

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

    glm::vec3 getPos() const { return m_pos; };
    void setPos(glm::vec3 pos){m_pos = pos;};
    Shader getShader() const { return *m_shader; };
    Model getObjModel() const { return *m_objModel; };
    glm::mat4 getModel() const;
    void initEnemy(Shader* shader, Model* model);
    void resetEnemy();
    bool collidePlayer(glm::vec3 playerPos) const;
    
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