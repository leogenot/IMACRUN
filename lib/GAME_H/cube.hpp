#ifndef CUBE_H
#define CUBE_H
#include "element.hpp"
#include "shader_m.hpp"


class Cube
{
private:
    Shader m_shader;
    unsigned int m_texture;
    unsigned int m_VAO, m_VBO;
    glm::vec3 m_pos;
public:
    void initCube();
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, float posX, float posY, float scale) const;
    //fonction de collision  
    //getter & setter pos
    glm::vec3 getPos() const {return m_pos;};
    float getPosX() const {return m_pos.x;};
    float getPosY() const {return m_pos.y;};
    float getPosZ() const {return m_pos.z;};
    void setPosX(const float posX) {m_pos.x = posX;};
    void setPosY(const float posY) {m_pos.y = posY;};
    void setPosZ(const float posZ) {m_pos.z = posZ;};
};

#endif