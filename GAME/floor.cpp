#include "GAME_H/floor.hpp"
#include "GAME_H/utilityFunction.hpp"

Floor::Floor()
{
    Shader shader("GAME/shaders/floor.vs", "GAME/shaders/floor.fs");
    m_shader = shader;

    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
        0.5f, -0.25f, 0.5f, 2.0f, 0.0f,
        -0.5f, -0.25f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.25f, -0.5f, 0.0f, 2.0f,

        0.5f, -0.25f, 0.5f, 2.0f, 0.0f,
        -0.5f, -0.25f, -0.5f, 0.0f, 2.0f,
        0.5f, -0.25f, -0.5f, 2.0f, 2.0f};

    // VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    
    glBindVertexArray(0);

    m_texture = loadTexture("assets/textures/floor/brickwall.jpg");
    m_shader.use();
    m_shader.setInt("texture1", 0);
}

void Floor::draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, int posX, int posY) const
{
    m_shader.use();

    m_shader.setMat4("view", view);
    m_shader.setMat4("projection", projection);
    model = glm::translate(model, glm::vec3(posX, 0, posY));
    m_shader.setMat4("model", model);

    glBindVertexArray(m_VAO);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}