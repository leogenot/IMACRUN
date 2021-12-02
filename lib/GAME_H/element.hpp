#ifndef ELEMENT_H
#define ELEMENT_H
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <glm/gtx/transform.hpp>
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glimac/Program.hpp"
#include "vector"
#include "light.hpp"
#include "sceneLight.hpp"

class Element
{
protected:
    glm::ivec3 m_pos;
public:
    Element(): canTurn(false) {};
    virtual void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, SceneLight sceneLight, std::vector<Light*> lights) const = 0;

    //getter & setter pos
    glm::ivec3 getPos() const {return m_pos;};
    int getPosX() const {return m_pos.x;};
    int getPosY() const {return m_pos.y;};
    int getPosZ() const {return m_pos.z;};
    void setPosX(const int posX) {m_pos.x = posX;};
    void setPosY(const int posY) {m_pos.y = posY;};
    void setPosZ(const int posZ) {m_pos.z = posZ;};

    //virtual fonction de collision
    bool possibleAdd; //if we can add another element (obsacle, money)
    bool canTurn; //if we are in an angle and we can turn
};

#endif