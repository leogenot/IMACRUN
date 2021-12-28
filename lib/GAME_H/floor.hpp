#ifndef FLOOR_H
#define FLOOR_H
#include "element.hpp"
#include "light.hpp"
#include "shader_m.h"

class Floor : public Element
{
private:
    Shader m_shader;
    unsigned int m_texture;
    unsigned int m_VAO;
public:
    Floor(bool angle, unsigned int texture);
    void resetElement() {possibleAdd = true;};
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, SceneLight sceneLight, std::vector<Light*> lights, glm::vec3 playerPos, int renderRadius) const override;
    Collision_Type collision() const {return NONE;};

};

#endif