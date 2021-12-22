#include "GAME_H/gamemap.hpp"
#include <chrono>
#include <random>
#include "GAME_H/utilityFunction.hpp"

void GameMap::loadGameMap(const std::string& path)
{
    //initialize tab of id
    m_textures.push_back(loadTexture<const char>("assets/textures/floor/rocks.jpg"));
    m_textures.push_back(loadTexture<const char>("assets/textures/cube/cube.jpg"));
    m_textures.push_back(loadTexture<const char>("assets/textures/cube/cube.jpg"));

    // open the file
    std::ifstream myfile;
    myfile.open(path, std::ios::in | std::ios::binary);

    // error
    if (!myfile.is_open()) {
        std::cout << "Can not open file: " << path << std::endl;
        return;
    }
    std::string type;
    myfile >> type;
    if (type.compare("P2") != 0) {
        std::cout << "PGM file should begin with P2" << std::endl;
        return;
    }

    // read size
    myfile >> m_sizeX;
    myfile >> m_sizeY;
    std::cout << m_sizeX << m_sizeY << std::endl;

    // read the data
    int data;
    for (int i = 0; i < m_sizeX * m_sizeY; ++i) {
        myfile >> data;
        switch (data) {
        case 255:
            m_grid.push_back(new Floor(false, m_textures[0]));
            break;
        case 155:
            m_grid.push_back(new Floor(true, m_textures[0]));
            break;
        case 100:
            m_grid.push_back(new Wall(m_textures[1]));
            break;
        case 0:
            m_grid.push_back(new Space);
            break;
        default:
            std::cout << "default" << std::endl;
            break;
        }
    }

    // close file
    myfile.close();

    //int midX = (int)(m_sizeX*0.5);
    //int midY = (int)(m_sizeY*0.5);

    for (int i = 0; i < m_sizeX; i++) {
        for (int j = 0; j < m_sizeY; j++) {
            // set position with i,j
            m_grid[i * m_sizeX + j]->setPosX(i);
            m_grid[i * m_sizeX + j]->setPosZ(j);
        }
    }
}

void GameMap::initObstacles(const int nbObstacles)
{
    int posX;
    int posY;
    //int midX = (int)(m_sizeX*0.5);
    //int midY = (int)(m_sizeY*0.5);

    // select seed from time
    unsigned seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
    // select a generator
    std::mt19937 generator(seed);
    // uniform int distribution
    std::uniform_int_distribution<int> uniformIntXDistrib(0, m_sizeX - 1);
    std::uniform_int_distribution<int> uniformIntYDistrib(0, m_sizeY - 1);

    for (int i = 0; i < nbObstacles; i++) {
        do {
            posX = uniformIntXDistrib(generator);
            posY = uniformIntYDistrib(generator);
        } while (!isEmpty(posX, posY)); // can't put obstacle (we want a floor with no obstacle)

        m_obstacles.push_back(new Obstacle(glm::vec3(posX, uniformIntXDistrib(generator) % 2 * 0.5, posY), m_textures[2])); //TODO: mieux gérer la hauteur des obstacles
    }
}

void GameMap::initLights(const int nbLights)
{
    int posX;
    int posY;
    //int midX =(int)(m_sizeX*0.5);
    //int midY =(int)(m_sizeY*0.5);

    // select seed from time
    unsigned seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
    // select a generator
    std::mt19937 generator(seed);
    // uniform int distribution
    std::uniform_int_distribution<int> uniformIntXDistrib(0, m_sizeX - 1);
    std::uniform_int_distribution<int> uniformIntYDistrib(0, m_sizeY - 1);
    // uniform real distribution
    std::uniform_real_distribution<float> uniformRealColorDistrib(0, 1);

    for (int i = 0; i < nbLights; i++) {
        do {
            posX = uniformIntXDistrib(generator);
            posY = uniformIntYDistrib(generator);
        } while (!isEmpty(posX, posY)); // can't put light (we want a floor with no obstacle)

        glm::vec3 pos(posX, 0, posY);
        glm::vec3  color(uniformRealColorDistrib(generator), uniformRealColorDistrib(generator), uniformRealColorDistrib(generator));
        m_lights.push_back(new Light(pos, color));

        m_grid[posX * m_sizeX + posY]->point       = true;
        m_grid[posX * m_sizeX + posY]->possibleAdd = false;
    }
}

bool GameMap::isEmpty(const int posX, const int posZ) const
{
    //int midX = (int)(m_sizeX*0.5);
    //int midY = (int)(m_sizeY*0.5);

    // if there is already an obstacle
    for (auto it = m_obstacles.begin(); it != m_obstacles.end(); it++) {
        if ((*it)->getPos().x == posX && (*it)->getPos().z == posZ)
            return false;
    }
    return m_grid[posX * m_sizeX + posZ]->possibleAdd;
}

bool GameMap::onAngle(const glm::vec3 pos) const
{
    return m_grid[round(pos.x) * m_sizeX + round(pos.z)]->canTurn; //test if player is on a turn case
};

bool GameMap::onPoint(const glm::vec3 pos)
{
    if (m_grid[round(pos.x) * m_sizeX + round(pos.z)]->point) {
        // Destruction of point
        m_grid[round(pos.x) * m_sizeX + round(pos.z)]->point = false;
        for (auto it = m_lights.begin(); it != m_lights.end(); it++) {
            if ((*it)->getPos().x == round(pos.x) && (*it)->getPos().z == round(pos.z)) {

                //std::remove(m_lights.begin(), m_lights.end(), *it);
                m_lights.erase(it);
                return true;
            }
        }
        return true;
    }
    return false;
};

bool GameMap::onObstacle(const glm::vec3 pos)
{
    for (auto it = m_obstacles.begin(); it != m_obstacles.end(); it++) {
        if ((*it)->getPos().x == round(pos.x) && (*it)->getPos().z == round(pos.z) && pos.y > (*it)->getPos().y - 0.2 && pos.y < (*it)->getPos().y + 0.2) // TODO : remplacer 0.2 par la taille de l'obstacle (hauteur)
        {
            //std::remove(m_obstacles.begin(), m_obstacles.end(), *it);
            m_obstacles.erase(it);
            std::cout << "Collision obstacle" << std::endl;
            return true;
        }
    }
    return false;
};

bool GameMap::collision(const glm::vec3 pos) const
{
    return m_grid[round(pos.x) * m_sizeX + round(pos.z)]->collision(); //test if player is on collision
};

void GameMap::destroyCollision(const glm::vec3 pos, glm::vec3 step)
{
    // destroy obstacles
    for (auto it = m_obstacles.begin(); it != m_obstacles.end(); it++) {
        if ((*it)->getPos().x == round(pos.x) && (*it)->getPos().z == round(pos.z))
        {
            m_obstacles.erase(it);
            std::cout << "destroy" << std::endl;
            break;
        }
        else if ((*it)->getPos().x == round(pos.x+step.x) && (*it)->getPos().z == round(pos.z+step.z))
        {
            m_obstacles.erase(it);
            std::cout << "destroy" << std::endl;
            break;
        }
        else if ((*it)->getPos().x == round(pos.x-step.x) && (*it)->getPos().z == round(pos.z-step.z))
        {
            m_obstacles.erase(it);
            std::cout << "destroy" << std::endl;
            break;
        }
    }

    // destroy lights
    for (auto it = m_lights.begin(); it != m_lights.end(); it++) {
        if ((*it)->getPos().x == round(pos.x) && (*it)->getPos().z == round(pos.z))
        {
            m_lights.erase(it);
            std::cout << "destroy" << std::endl;
            break;
        }
        else if ((*it)->getPos().x == round(pos.x+step.x) && (*it)->getPos().z == round(pos.z+step.z))
        {
            m_lights.erase(it);
            std::cout << "destroy" << std::endl;
            break;
        }
        else if ((*it)->getPos().x == round(pos.x-step.x) && (*it)->getPos().z == round(pos.z-step.z))
        {
            m_lights.erase(it);
            std::cout << "destroy" << std::endl;
            break;
        }
    }
}

void GameMap::drawGameMap(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, Model lightning_bolt) 
{
    //draw path
    for (auto it = m_grid.begin(); it != m_grid.end(); it++)
        (*it)->draw(view, projection, model, camPos, m_sceneLight, m_lights);

    //draw obstacles
    for (auto it = m_obstacles.begin(); it != m_obstacles.end(); it++)
        (*it)->draw(view, projection, model, camPos, m_sceneLight, m_lights);

    //draw lights
    for (auto it = m_lights.begin(); it != m_lights.end(); it++)
        (*it)->draw(view, projection, model, 0.2f, lightning_bolt);
}
