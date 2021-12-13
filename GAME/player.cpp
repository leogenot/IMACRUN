#include "GAME_H/player.hpp"
#include "GAME_H/utilityFunction.hpp"

# define M_PI           3.14159265358979323846

void Player::initPlayer()
{
    Shader shader("GAME/shaders/model_loading.vs", "GAME/shaders/model_loading.fs");
    m_shader = shader;
}

void Player::draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, Model modelObj) 
{
    m_shader.use();

    m_shader.setMat4("view", view);
    m_shader.setMat4("projection", projection);
    model = glm::translate(model, m_pos);
    model = glm::rotate(model, -90-Yaw, glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(.2f, .2f, .2f));
    m_shader.setMat4("model", model);
    modelObj.DrawModel(m_shader);
    
}

void Player::Jump()
{
    if (onGround) {
        isFalling = false;
        onGround  = false;
        //      updatePlayerVectors();
    }
}
void Player::Rise(float deltatime)
{
    if (m_pos.y <= PLAYERJUMPHEIGHT) {
        m_pos.y += (1.5f * deltatime);
    }
    else {
        isFalling = true;
    }
}
void Player::Fall(float deltatime)
{
    if (m_pos.y <= PLAYERSTART[1] + 0.05f) {
        m_pos.y = PLAYERSTART[1];
        onGround   = true;
    }
    else {
        m_pos.y -= (1.5f * deltatime);
    }
    //updatePlayerVectors();
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Player::ProcessKeyboard(Camera_Movement direction, float deltaTime, GameMap gamemap)
{
    bool  positionChanged = true;
    float velocity        = MovementSpeed * deltaTime;
    if(direction == FORWARD && gamemap.collision(m_pos))
        std::cout << "THE END" << std::endl;
    else if(direction == FORWARD)
        m_pos += Front * velocity;

    if (direction == LEFT && !gamemap.collision(m_pos - Right))
        m_pos -= Right; // * velocity; //TODO : transition
    if (direction == RIGHT && !gamemap.collision(m_pos + Right))
        m_pos += Right; // * velocity; //TODO : transition

    if (direction == ROTATELEFT)
    {
        Yaw -= 90; // player rotation
        m_camera->setDirection(Yaw);
        m_pos = glm::ivec3(m_pos); // to make the player stay in a case and not in between
    }
    if (direction == ROTATERIGHT)
    {
        Yaw += 90; // player rotation
        m_camera->setDirection(Yaw);
        m_pos = glm::ivec3(m_pos); // to make the player stay in a case and not in between
    }

    if (onGround) {
        // feet on the floor
        m_pos.y = PLAYERSTART[1];
    }
    else if (!onGround && !isFalling) {
        //rising
    }
    else {
        //falling
    }

    // check collision
    if (m_pos.x > 16.f || m_pos.z > 16.f ||
        m_pos.x < 1 || m_pos.z < -1) {
        if (direction == FORWARD) {
            m_pos -= Front * velocity;
        }
        if (direction == BACKWARD) {
            m_pos += Front * velocity;
        }
        if (direction == LEFT) {
            m_pos += Right;
        }
        if (direction == RIGHT) {
            m_pos -= Right;
        }
        positionChanged = false;
    }

    // check light collision
    if(gamemap.onPoint(m_pos))
        addScore();

    updatePlayerVectors();
    //std::cout << "X: " << m_pos.x << " " << "Y: " << m_pos.y << " " << "Z: "<< m_pos.z << std::endl;
}

void Player::updatePlayerVectors()
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

void Player::ResetPlayer(){
    glm::vec3 pos = glm::vec3(PLAYERSTART[0], PLAYERSTART[1], PLAYERSTART[2]);
    m_pos = pos;
    updatePlayerVectors();
}