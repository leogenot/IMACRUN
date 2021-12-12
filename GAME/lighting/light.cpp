#include "GAME_H/light.hpp"
#include "GAME_H/utilityFunction.hpp"

Light::Light(const glm::ivec3 pos, const glm::vec3 color) : m_pos(pos), m_color(color)
{
    Shader shader("GAME/shaders/lightSource.vs", "GAME/shaders/lightSource.fs");
    m_shader = shader;

    float cubeVertices[] = {
        // positions       
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f};

    // VAO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_shader.use();
}

void Light::draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, float scale) const
{
    m_shader.use();

    m_shader.setMat4("view", view);
    m_shader.setMat4("projection", projection);
    model = glm::translate(model, glm::vec3(m_pos.x, m_pos.y, m_pos.z));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    m_shader.setMat4("model", model);

    m_shader.setVec3("lightColor",  m_color);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}