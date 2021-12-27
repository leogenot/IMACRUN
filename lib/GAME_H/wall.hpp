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
    Wall(unsigned int texture);
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, SceneLight sceneLight, std::vector<Light*> lights, glm::vec3 playerPos) const override;
    Collision_Type collision() const {return COLLIDE;};
};

#endif