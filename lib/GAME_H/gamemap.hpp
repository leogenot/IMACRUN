#ifndef GAMEMAP_H
#define GAMEMAP_H
#include "element.hpp"
#include "space.hpp"
#include "floor.hpp"
#include "wall.hpp"
#include "obstacle.hpp"
#include "light.hpp"
#include "sceneLight.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class GameMap
{
private: 
    int m_sizeX;
    int m_sizeY;
    std::vector<Element*> m_grid;
    std::vector<Obstacle*> m_obstacles;
    std::vector<Light*> m_lights;
    SceneLight m_sceneLight;
    std::vector<unsigned int> m_textures;

public:
    GameMap(SceneLight sceneLight) : m_sceneLight(sceneLight) {};
    void loadGameMap(const std::string &path);
    void drawGameMap(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos) const;

    void initObstacles(const int nbObstacles);
    void initLights(const int nbLights);
    bool isEmpty(const int posX, const int posY) const;
    bool onAngle(const glm::vec3 pos) const;
    bool collision(const glm::vec3 pos) const;
};

#endif