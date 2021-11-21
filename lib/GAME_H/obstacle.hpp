#ifndef OBSTACLE_H
#define OBSTACLE_H
#include "shader_m.h"
#include "light.hpp"
#include "sceneLight.hpp"
#include <vector>

class Obstacle
{
private:
    Shader m_shader;
    unsigned int m_texture;
    unsigned int m_VAO;
    glm::vec3 m_pos;

public:
    Obstacle(glm::vec3 pos);
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, SceneLight sceneLight, std::vector<Light*> lights) const;
    float getPosX() const {return m_pos.x;};
    float getPosZ() const {return m_pos.z;};
    //fonction de collision  
};

#endif