#include "GAME_H/player/enemy.hpp"

void Enemy::initEnemy(Shader* shader, Model* model)
{
    m_shader = shader;
    m_objModel = model;
}

void Enemy::resetEnemy()
{
    // Reset enemy info
    m_pos = glm::vec3(ENEMYSTART[0], ENEMYSTART[1], ENEMYSTART[2]);
    onGround = true;
    isFalling = false;

    // Reset angle and vector
    Yaw = YAW;
    Pitch = PITCH;
    MovementSpeed = ENEMYSPEED;
    WorldUp = glm::vec3(0., 1., 0.);
    Front = glm::vec3(0., 0., -1.);
    updateEnemyVectors();
}

bool Enemy::collidePlayer(glm::vec3 playerPos) const
{
    if (round(playerPos.x) == round(getPos().x) && round(playerPos.z) == round(getPos().z))
        return true;
    else if (round(playerPos.x) == round(getPos().x + Right.x) && round(playerPos.z) == round(getPos().z + Right.z))
        return true;
    else if (round(playerPos.x) == round(getPos().x - Right.x) && round(playerPos.z) == round(getPos().z - Right.z))
        return true;

    return false;
}

glm::mat4 Enemy::getModel() const
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_pos);
    model = glm::rotate(model, -Yaw, glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(.3f, .3f, .3f));
    return model;
}

void Enemy::Jump()
{
    if (onGround) {
        isFalling = false;
        onGround  = false;
    }
}

void Enemy::Rise(float deltatime)
{
    if (m_pos.y <= ENEMYJUMPHEIGHT) {
        m_pos.y += (1.5f * deltatime);
    }
    else {
        isFalling = true;
    }
}

void Enemy::Fall(float deltatime)
{
    if (m_pos.y <= ENEMYSTART[1] + 0.05f) {
        m_pos.y = ENEMYSTART[1];
        onGround   = true;
    }
    else {
        m_pos.y -= (1.5f * deltatime);
    }
}

void Enemy::Follow(GameMap *gamemap, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    m_pos += Front * velocity;

    // Auto turn
    if (gamemap->onAngle(m_pos) && gamemap->collision(m_pos+Front+Front) == COLLIDE) // if their is a wall in front of him
    {
        if (gamemap->collision(m_pos+Right+Right) == COLLIDE) // if their is a wall then the rotation need to be in the other side
        {
            Yaw -= 90; // enemy rotation
            m_pos = glm::ivec3(round(m_pos.x), round(m_pos.y), round(m_pos.z)); // to make the enemy stay in a case and not in between
        }
        else
        {
            Yaw += 90; // enemy rotation
            m_pos = glm::ivec3(round(m_pos.x), round(m_pos.y), round(m_pos.z)); // to make the enemy stay in a case and not in between
        }
    }

    // Auto jump
    if (gamemap->collision(m_pos+Front) == FALL)
        Jump();
    if (!onGround && !isFalling) //then rising
        Rise(deltaTime);
    else if (!onGround)
        Fall(deltaTime);

    // check light and obstacle collision to destroy them
    gamemap->destroyCollision(m_pos, Right);

    updateEnemyVectors();
}

void Enemy::updateEnemyVectors()
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
};