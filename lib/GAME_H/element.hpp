#ifndef ELEMENT_H
#define ELEMENT_H
#include <glm/glm.hpp>
#include <iostream>

class Element
{
public:
    Element() {}
    virtual void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, int posX, int posY) const = 0;    
    //virtual fonction de collision
};

#endif