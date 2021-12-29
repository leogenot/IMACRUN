#include "GAME_H/light.hpp"
#include "GAME_H/utilityFunction.hpp"

void Light::draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model)
{
    m_shader->use();

    m_shader->setVec3("lightColor", m_color);
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);
    
    model = glm::translate(model, m_pos);
    model = glm::translate(model, glm::vec3(0.0f, .1f, 0.0f));
    model = glm::rotate(model,90.0f,glm::vec3(1,0,0));
    model = glm::scale(model, glm::vec3(.04f, .04f, .04f));
    m_shader->setMat4("model", model);
    m_objModel->DrawModel(*m_shader);
}

glm::mat4 Light::getModel() const
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_pos);
    model = glm::translate(model, glm::vec3(0.0f, .1f, 0.0f));
    model = glm::rotate(model, 90.0f, glm::vec3(1,0,0));
    model = glm::scale(model, glm::vec3(.04f, .04f, .04f));
    return model;
}