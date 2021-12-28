#include "GAME_H/light.hpp"
#include "GAME_H/utilityFunction.hpp"

Light::Light(const glm::vec3 pos, const glm::vec3 color) : m_pos(pos), m_color(color)
{
    Shader shader("GAME/shaders/lightSource.vs", "GAME/shaders/lightSource.fs");

    m_shader = shader;

}

void Light::draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, float scale, Model lightning_bolt)
{
    m_shader.use();

    m_shader.setVec3("lightColor", m_color);
    m_shader.setMat4("view", view);
    m_shader.setMat4("projection", projection);
    
    model = glm::translate(model, m_pos);
    model = glm::translate(model, glm::vec3(0.0f, .1f, 0.0f));
    model = glm::rotate(model,90.0f,glm::vec3(1,0,0));
    model = glm::scale(model, glm::vec3(.04f, .04f, .04f));
    m_shader.setMat4("model", model);
    lightning_bolt.DrawModel(m_shader);
}