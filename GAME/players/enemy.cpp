#include "GAME_H/enemy.hpp"

void Enemy::initEnemy()
{
    Shader shader("GAME/shaders/model_loading.vs", "GAME/shaders/model_loading.fs");
    m_shader = shader;
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

void Enemy::drawEnemy(glm::mat4 view, glm::mat4 projection, glm::mat4 model, Model objModel) 
{
    m_shader.use();

    m_shader.setMat4("view", view);
    m_shader.setMat4("projection", projection);
    model = glm::translate(model, m_pos);
    model = glm::rotate(model, -Yaw, glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(.2f, .2f, .2f));
    m_shader.setMat4("model", model);
    objModel.DrawModel(m_shader);
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

    if (gamemap->onAngle(m_pos) && gamemap->collision(m_pos+Front+Front)) 
    {
        if (gamemap->collision(m_pos+Right+Right)) // if their is a wall then the rotation need to be in the other side
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

    //if (gamemap->inSpace()) //TODO in space
    //    Jump();

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