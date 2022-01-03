#ifndef OBSTACLE_H
#define OBSTACLE_H
#include "../utilities/shader_m.hpp"
#include "../lighting/light.hpp"
#include "../lighting/sceneLight.hpp"
#include <vector>

class Obstacle
{
private:
    Shader m_shader;
    unsigned int m_texture;
    unsigned int m_VAO;
    glm::vec3 m_pos;

public:
    Obstacle(glm::vec3 pos, unsigned int texture);
    ~Obstacle() { glDeleteVertexArrays(1, &m_VAO); };
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, SceneLight sceneLight, std::vector<Light*> lights, glm::vec3 playerPos, int renderRadius) const;
    glm::vec3 getPos() const {return m_pos;}; 
};

#endif