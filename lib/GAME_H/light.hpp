#ifndef LIGHT_H
#define LIGHT_H
#include "shader_m.h"
#include <glm/glm.hpp>
#include <vector>
#include "model.hpp"

class Light
{
private:
    Shader* m_shader;
    Model* m_objModel;
    glm::vec3 m_pos;
    glm::vec3 m_color;

public:
    Light::Light(const glm::vec3 pos, const glm::vec3 color, Shader* shader, Model* model) : m_pos(pos), m_color(color), m_shader(shader), m_objModel(model) {};
    ~Light() {};
    glm::vec3 getPos() const {return m_pos;};
    glm::vec3 getColor() const {return m_color;};
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, float scale);
};

#endif