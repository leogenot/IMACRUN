#ifndef FLOOR_H
#define FLOOR_H
#include "element.hpp"
#include "shader_m.h"

class Floor : public Element
{
private:
    Shader m_shader;
    unsigned int m_texture;
    unsigned int m_VAO;
public:
    Floor();
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, int posX, int posY) const;
    //fonction de collision  
};

#endif