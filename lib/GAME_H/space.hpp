#ifndef SPACE_H
#define SPACE_H
#include "element.hpp"

class Space : public Element
{
public:
    Space() {possibleAdd = false;};
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, SceneLight sceneLight, std::vector<Light*> lights) const override{};
    bool collision() const {return false;};
};

#endif