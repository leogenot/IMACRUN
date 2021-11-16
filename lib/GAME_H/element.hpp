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
public:
    Element() {}
    virtual void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, int posX, int posY) const = 0;    
    //virtual fonction de collision
    bool possibleAdd; //if we can add another element (obsacle, money)
};

#endif