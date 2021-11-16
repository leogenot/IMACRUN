#ifndef MAP_H
#define MAP_H
#include "element.hpp"
#include "space.hpp"
#include "floor.hpp"
#include "wall.hpp"
#include "obstacle.hpp"
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

public:
    Map() {};
    void loadMap(const std::string &path);
    void drawMap(glm::mat4 view, glm::mat4 projection, glm::mat4 model);

    void initObstacles(const int nbObstacles);
    bool isEmpty(float posX, float posY);
};

#endif