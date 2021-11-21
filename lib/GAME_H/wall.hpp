#ifndef WALL_H
#define WALL_H
#include "element.hpp"
#include "shader_m.h"

class Wall : public Element
{
private:
    Shader m_shader;
    unsigned int m_texture;
    unsigned int m_VAO;
public:
    Wall();
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, glm::vec3 lightDir, std::vector<Light*> lights) const;
    //fonction de collision  
};

#endif