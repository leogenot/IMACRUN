#ifndef SCENELIGHT_H
#define SCENELIGHT_H
#include <glm/glm.hpp>

class SceneLight
{
private:
    glm::vec3 m_direction;
    glm::vec3 m_color;
public:
    SceneLight() {};
    SceneLight(const glm::vec3 direction, const glm::vec3 color) : m_direction(direction), m_color(color) {};
    glm::vec3 getDirection() const {return m_direction;};
    glm::vec3 getColor() const {return m_color;};
};

#endif