#ifndef LIGHT_H
#define LIGHT_H
#include "shader_m.h"


class Light
{
private:
    Shader m_shader;
    unsigned int m_VAO, m_VBO;
    glm::vec3 m_pos;
    glm::vec3 m_color;
public:
    Light(glm::vec3 pos, glm::vec3 color);
    glm::vec3 getPos() const {return m_pos;};
    glm::vec3 getColor() const {return m_color;};
    //void initLight();
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, float scale) const;
};

#endif