#include "GAME_H/gamemap/gamemap.hpp"
#include <chrono>
#include <random>
#include "GAME_H/utilities/utilityFunction.hpp"

void GameMap::loadGameMap(const std::string& path)
{
    //initialize tab of id
    m_textures.push_back(loadTexture<const char>("assets/textures/floor/sand.jpg"));
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

    // free unused memory
    m_grid.shrink_to_fit();

    // close file
    myfile.close();

    for (int i = 0; i < m_sizeX; i++) {
        for (int j = 0; j < m_sizeY; j++) {
            // set position with i,j
            m_grid[i * m_sizeX + j]->setPosX(i);
            m_grid[i * m_sizeX + j]->setPosZ(j);
        }
    }
}

void GameMap::resetGameMap(int nbObstacles, int nbLights, Shader* shader, Model* model)
{
    // reset grid (possibleAdd and point)
    for (auto it = m_grid.begin(); it != m_grid.end(); it++)
        (*it)->resetElement();

    // reset obstacles
    m_obstacles.clear();
    initObstacles(nbObstacles);

    // reset lights
    m_lights.clear();
    initLights(nbLights, shader, model);
}

void GameMap::initObstacles(const int nbObstacles)
{
    int posX, posY;

    // select seed from time
    unsigned seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
    // select a generator
    std::mt19937 generator(seed);
    // uniform int distribution
    std::uniform_int_distribution<int> uniformIntXDistrib(0, m_sizeX - 1);
    std::uniform_int_distribution<int> uniformIntYDistrib(0, m_sizeY - 1);
    std::uniform_int_distribution<int> uniformIntDistrib(0, 1);

    for (int i = 0; i < nbObstacles; i++) {
        do {
            posX = uniformIntXDistrib(generator);
            posY = uniformIntYDistrib(generator);
        } while (!isEmpty(posX, posY)); // can't put obstacle (we want a floor with no obstacle)

        m_obstacles.push_back(new Obstacle(glm::vec3(posX, uniformIntDistrib(generator), posY), m_textures[2])); //TODO: mieux gérer la hauteur des obstacles
    }
}

void GameMap::initLights(const int nbLights, Shader* shader, Model* model)
{
    int posX, posY;

    int Xmax = m_sizeX - 1;
    int Ymax = m_sizeY - 1;

    // select seed from time
    unsigned seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
    // select a generator
    std::mt19937 generator(seed);
    // uniform int distribution
    std::uniform_int_distribution<int> uniformIntXDistrib(0, Xmax);
    std::uniform_int_distribution<int> uniformIntYDistrib(0, Ymax);
    std::uniform_int_distribution<int> uniformIntValueDistrib(1, 3);

    for (int i = 0; i < nbLights; i++) {
        // pos
        do {
            posX = uniformIntXDistrib(generator);
            posY = uniformIntYDistrib(generator);
        } while (!isEmpty(posX, posY)); // can't put light (we want a floor with no obstacle)

        glm::vec3 pos(posX, 0, posY);

        // value
        int value = uniformIntValueDistrib(generator);

        // color
        glm::vec3 color;
        if (value == 1)
            color = glm::vec3(0.8f, 0.8f, 0.0f);
        else if (value == 2)
            color = glm::vec3(1.0f, 0.5f, 0.0f);
        else
            color = glm::vec3(0.6f, 0.7f, 0.9f);

        m_lights.push_back(new Light(pos, color, value, shader, model));

        m_grid[posX * m_sizeX + posY]->point       = true;
        m_grid[posX * m_sizeX + posY]->possibleAdd = false;
    }
}

bool GameMap::isEmpty(const int posX, const int posZ) const
{
    // if there is already an obstacle
    for (auto it = m_obstacles.begin(); it != m_obstacles.end(); it++) {
        if ((*it)->getPos().x == posX && (*it)->getPos().z == posZ)
            return false;
    }
    return m_grid[posX * m_sizeX + posZ]->possibleAdd;
}

bool GameMap::onAngle(const glm::vec3 pos) const
{
    return m_grid[(int)round(pos.x) * m_sizeX + (int)round(pos.z)]->canTurn; //test if player is on a turn case
};

int GameMap::getPoint(const glm::vec3 pos)
{
    int value = 0;
    const unsigned int indice = (int)round(pos.x) * m_sizeX + (int)round(pos.z);
    if (m_grid[indice]->point) {
        // Destruction of point
        m_grid[indice]->point = false;
        for (auto it = m_lights.begin(); it != m_lights.end(); it++) {
            if ((*it)->getPos().x == round(pos.x) && (*it)->getPos().z == round(pos.z)) {
                value = (*it)->getValue();
                m_lights.erase(it);
                return value;
            }
        }
        return value;
    }
    return value;
};

bool GameMap::onObstacle(const glm::vec3 pos, bool down)
{
    for (auto it = m_obstacles.begin(); it != m_obstacles.end(); it++) {
        if ((*it)->getPos().x == round(pos.x) && (*it)->getPos().z == round(pos.z)/*pos.y > (*it)->getPos().y - 0.2 && pos.y < (*it)->getPos().y + 0.2*/) // TODO : remplacer 0.2 par la taille de l'obstacle (hauteur)
        {
            if((*it)->getPos().y == 1 && !down)
            {
                m_obstacles.erase(it);
                std::cout << "Collision obstacle up" << std::endl;
                return true;
            }
            else if((*it)->getPos().y == pos.y)
            {
                m_obstacles.erase(it);
                std::cout << "Collision obstacle down" << std::endl;
                return true;
            }
        }
    }
    return false;
};

Collision_Type GameMap::collision(const glm::vec3 pos) const
{
    return m_grid[(int)round(pos.x) * m_sizeX + (int)round(pos.z)]->collision(); //test if player is on collision
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

void GameMap::drawGameMap(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, glm::vec3 playerPos, int renderRadius) 
{
    //draw path
    for (auto it = m_grid.begin(); it != m_grid.end(); it++)
    {
        if ((*it)->getPos().x < playerPos.x + renderRadius && (*it)->getPos().x > playerPos.x - renderRadius && (*it)->getPos().z < playerPos.z + renderRadius && (*it)->getPos().z > playerPos.z - renderRadius)
            (*it)->draw(view, projection, model, camPos, m_sceneLight, m_lights, playerPos, renderRadius);
    }

    //draw obstacles
    for (auto it = m_obstacles.begin(); it != m_obstacles.end(); it++)
    {
        if ((*it)->getPos().x < playerPos.x + renderRadius && (*it)->getPos().x > playerPos.x - renderRadius && (*it)->getPos().z < playerPos.z + renderRadius && (*it)->getPos().z > playerPos.z - renderRadius)
            (*it)->draw(view, projection, model, camPos, m_sceneLight, m_lights, playerPos, renderRadius);
    }

    //draw lights
    for (auto it = m_lights.begin(); it != m_lights.end(); it++)
    {
        if ((*it)->getPos().x < playerPos.x + renderRadius && (*it)->getPos().x > playerPos.x - renderRadius && (*it)->getPos().z < playerPos.z + renderRadius && (*it)->getPos().z > playerPos.z - renderRadius)
            (*it)->draw(view, projection, model);
    }
}
