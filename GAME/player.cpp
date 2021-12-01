#include "GAME_H/player.hpp"
#include "GAME_H/utilityFunction.hpp"

void Player::initCube()
{
    Shader shader("GAME/shaders/floor.vs", "GAME/shaders/floor.fs");
    m_shader = shader;

    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    // VAO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    m_texture = loadTexture<const char>("assets/textures/cube/cube.jpg");
    m_shader.use();
    m_shader.setInt("texture1", 0);
}

void Player::draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model) const
{
    m_shader.use();

    m_shader.setMat4("view", view);
    m_shader.setMat4("projection", projection);
    model = glm::translate(model, m_pos);
    model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
    m_shader.setMat4("model", model);

    glBindVertexArray(m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Player::Jump()
{
    if (onGround) {
        isFalling = false;
        onGround  = false;
        //      updateCameraVectors();
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
    //updateCameraVectors();
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Player::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    bool  positionChanged = true;
    float velocity        = m_camera->MovementSpeed * deltaTime;
   if (direction == LEFT)
        m_pos -= Right * velocity; //on veut se déplacer d'une seule case sinon on multiplie par velocity
    if (direction == RIGHT)
        m_pos += Right * velocity;
    if (direction == FORWARD)
        m_pos += Front * velocity;
    if (direction == ROTATELEFT)
    {
        Yaw = Yaw + 90;
        m_camera->Yaw += 90;
        m_camera->MAXYAWANGLE += 90;
        m_camera->MINYAWANGLE += 90;
        m_camera->updateCameraVectors();
    }
    if (direction == ROTATERIGHT)
    {
        Yaw = Yaw - 90;
        m_camera->Yaw -= 90;
        m_camera->MAXYAWANGLE -= 90;
        m_camera->MINYAWANGLE -= 90;
        m_camera->updateCameraVectors();
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

void Player::updateCameraVectors()
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