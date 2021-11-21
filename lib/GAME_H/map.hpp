#ifndef MAP_H
#define MAP_H
#include "element.hpp"
#include "space.hpp"
#include "floor.hpp"
#include "wall.hpp"
#include "obstacle.hpp"
#include "light.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class Map
{
private: 
    int m_sizeX;
    int m_sizeY;
    std::vector<Element*> m_grid;
    std::vector<Obstacle*> m_obstacles;
    std::vector<Light*> m_lights;

public:
    Map() {};
    void loadMap(const std::string &path);
    void drawMap(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, glm::vec3 lightDir);

    void initObstacles(const int nbObstacles);
    void initLights(const int nbLights);
    bool isEmpty(int posX, int posY);
};

#endif