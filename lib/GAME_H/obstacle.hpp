#ifndef OBSTACLE_H
#define OBSTACLE_H
#include "shader_m.h"

class Obstacle
{
private:
    Shader m_shader;
    unsigned int m_texture;
    unsigned int m_VAO;
    float m_posX;
    float m_posY;
    int m_level; //gérer si l'obsacle est en haut ou au sol
public:
    Obstacle(float posX, float posY, int level);
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model) const;
    float getPosX() {return m_posX;};
    float getPosY() {return m_posY;};
    //fonction de collision  
};

#endif