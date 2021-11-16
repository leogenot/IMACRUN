#ifndef CUBE_H
#define CUBE_H
#include "element.hpp"
#include "shader_m.h"


class Cube
{
private:
    Shader m_shader;
    unsigned int m_texture;
    unsigned int m_VAO, m_VBO;
public:
    void initCube();
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, float posX, float posY, float scale) const;
    //fonction de collision  
};

#endif