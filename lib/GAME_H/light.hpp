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
    int m_value;

public:
    Light(const glm::vec3 pos, const glm::vec3 color, int value, Shader* shader, Model* model) : m_pos(pos), m_color(color), m_value(value), m_shader(shader), m_objModel(model) {};
    ~Light() {};
    glm::vec3 getPos() const { return m_pos; };
    glm::vec3 getColor() const { return m_color; };
    int getValue() const { return m_value; };
    Shader getShader() const { return *m_shader; };
    Model getObjModel() const { return *m_objModel; };
    glm::mat4 getModel() const;
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model);
};

#endif