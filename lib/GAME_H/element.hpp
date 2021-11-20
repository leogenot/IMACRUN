#ifndef ELEMENT_H
#define ELEMENT_H
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glimac/Program.hpp"

class Element
{
protected:
    glm::vec3 m_pos;
public:
    Element() {};
    virtual void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model) const = 0;

    //getter & setter pos
    glm::vec3 getPos() const {return m_pos;};
    float getPosX() const {return m_pos.x;};
    float getPosY() const {return m_pos.y;};
    float getPosZ() const {return m_pos.z;};
    void setPosX(float posX) {m_pos.x = posX;};
    void setPosY(float posY) {m_pos.y = posY;};
    void setPosZ(float posZ) {m_pos.z = posZ;};

    //virtual fonction de collision
    bool possibleAdd; //if we can add another element (obsacle, money)
};

#endif