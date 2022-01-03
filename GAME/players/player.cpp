#include "GAME_H/player/player.hpp"

void Player::initPlayer(Shader* shader, Model* model)
{
    m_shader   = shader;
    m_objModel = model;
}

glm::mat4 Player::getModel() const
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_pos);
    model           = glm::rotate(model, -90 - Yaw, glm::vec3(0, 1, 0));
    model           = glm::scale(model, glm::vec3(.25f, .25f, .25f));
    if (down)
        model = glm::scale(model, glm::vec3(1.0f, .25f, 1.0f));

    return model;
}

void Player::Jump()
{
    if (onGround) {
        isFalling = false;
        onGround  = false;
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
        m_pos.y  = PLAYERSTART[1];
        onGround = true;
    }
    else {
        m_pos.y -= (1.5f * deltatime);
    }
}

bool Player::getCollision(Camera_Movement direction, GameMap* gamemap)
{
    if (direction == FORWARD && gamemap->collision(m_pos) == COLLIDE || gamemap->collision(m_pos) == FALL && onGround) {
        cout << "COllision" << endl;
        return true;
    }
    else
        return false;
}
// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Player::ProcessKeyboard(Camera_Movement direction, float deltaTime, GameMap* gamemap)
{
    bool  positionChanged = true;
    float velocity        = MovementSpeed * deltaTime;

    if (direction == FORWARD)
        m_pos += Front * velocity;

    if (direction == LEFT) {
        if (gamemap->onAngle(m_pos)) // rotate
        {
            Yaw -= 90; // player rotation
            m_camera->setDirection(Yaw);
            m_pos = glm::ivec3(round(m_pos.x), round(m_pos.y), round(m_pos.z)); // to make the player stay in a case and not in between
        }
        else if (!gamemap->collision(m_pos - Right)) // translate
            m_pos -= Right;                          // * velocity; //TODO : transition
    }
    else if (direction == RIGHT) {
        if (gamemap->onAngle(m_pos)) // rotate
        {
            Yaw += 90; // player rotation
            m_camera->setDirection(Yaw);
            m_pos = glm::ivec3(round(m_pos.x), round(m_pos.y), round(m_pos.z)); // to make the player stay in a case and not in between
        }
        else if (!gamemap->collision(m_pos + Right)) //translate
            m_pos += Right;                          // * velocity; //TODO : transition
    }

    // jump
    if (!onGround && !isFalling) //then rising
        Rise(deltaTime);
    else if (!onGround) //then falling
        Fall(deltaTime);

    // check light collision to increment score
    addScore(gamemap->getPoint(m_pos));


    // check obstacle collision
    if (gamemap->onObstacle(m_pos, down))
        removeLife();

    updatePlayerVectors();
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

void Player::resetPlayer()
{
    // Reset player info
    m_pos     = glm::vec3(PLAYERSTART[0], PLAYERSTART[1], PLAYERSTART[2]);
    onGround  = true;
    isFalling = false;
    m_score   = 0;
    m_life    = 5;

    // Reset angle and vector
    Yaw           = YAW;
    Pitch         = PITCH;
    MovementSpeed = SPEED;
    WorldUp       = glm::vec3(0., 1., 0.);
    Front         = glm::vec3(0., 0., -1.);
    updatePlayerVectors();
}

void Player::ShowPlayerData(Player* m_player)
{
    cout << m_player->m_username << " : Score : " << m_player->m_score << " Life : " << m_player->m_life << endl;
}