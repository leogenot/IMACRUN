#ifndef MAP_H
#define MAP_H
#include "element.hpp"
#include "space.hpp"
#include "floor.hpp"

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

public:
    Map() {};
    void loadMap(const std::string &path);
    void drawMap(glm::mat4 view, glm::mat4 projection, glm::mat4 model);
};

#endif