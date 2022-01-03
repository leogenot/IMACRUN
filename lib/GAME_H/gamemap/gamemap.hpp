#ifndef GAMEMAP_H
#define GAMEMAP_H
#include "../gamemap/element.hpp"
#include "../gamemap/space.hpp"
#include "../gamemap/floor.hpp"
#include "../gamemap/wall.hpp"
#include "../gamemap/obstacle.hpp"
#include "../lighting/light.hpp"
#include "../lighting/sceneLight.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <functional>

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
    GameMap() {};
    GameMap(SceneLight sceneLight) : m_sceneLight(sceneLight) {};
    void loadGameMap(const std::string &path);
    void resetGameMap(int nbObstacles, int nbLights, Shader* shader, Model* model);
    void drawGameMap(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, glm::vec3 playerPos, int renderRadius) ;

    void initObstacles(const int nbObstacles);
    void initLights(const int nbLights, Shader* shader, Model* model);
    SceneLight getSceneLight() {return m_sceneLight;};
    std::vector<Light*> getLights() {return m_lights;};
    
    bool isEmpty(const int posX, const int posY) const; // we want a floor with no obstacle
    bool onAngle(const glm::vec3 pos) const; // if player on a turning point
    int getPoint(const glm::vec3 pos); // if player on light return light value
    bool onObstacle(const glm::vec3 pos, bool down); // if player collide an obstacle
    Collision_Type collision(const glm::vec3 pos) const; // return type of collision (fall, collide or none)
    void destroyCollision(const glm::vec3 pos, glm::vec3 step); //destroy light and obstacle on collision with the enemy
};

#endif