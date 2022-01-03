#ifndef ELEMENT_H
#define ELEMENT_H
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <glm/gtx/transform.hpp>
#include "../../stb_image.h"
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "vector"
#include "../lighting/light.hpp"
#include "../lighting/sceneLight.hpp"

enum Collision_Type {
    NONE,
    FALL,
    COLLIDE
};

class Element
{
protected:
    glm::ivec3 m_pos;
public:
    bool possibleAdd; //if we can add another element (obsacle, point)
    bool canTurn; //if we are in an angle and we can turn
    bool point; //if their is a point

    Element(): canTurn(false), point(false) {};
    void resetElement() {point = false;};
    virtual void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, SceneLight sceneLight, std::vector<Light*> lights, glm::vec3 playerPos, int renderRadius) const = 0;

    //getter & setter pos
    glm::ivec3 getPos() const {return m_pos;};
    void setPosX(const int posX) {m_pos.x = posX;};
    void setPosY(const int posY) {m_pos.y = posY;};
    void setPosZ(const int posZ) {m_pos.z = posZ;};
    virtual Collision_Type collision() const = 0;
};

#endif