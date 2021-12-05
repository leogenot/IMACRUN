#include "GAME_H/gamemap.hpp"
#include <random>
#include <chrono>

void GameMap::loadGameMap(const std::string &path)
{
    // open the file
    std::ifstream myfile;
    myfile.open(path, std::ios::in | std::ios::binary);

    // error
    if (!myfile.is_open())
    {
        std::cout << "Can not open file: " << path << std::endl;
        return;
    }
    std::string type;
    myfile >> type;
    if (type.compare("P2") != 0)
    {
        std::cout << "PGM file should begin with P2" << std::endl;
        return;
    } 

    // read size
    myfile >> m_sizeX;
    myfile >> m_sizeY;
    std::cout << m_sizeX << m_sizeY << std::endl;

    // read the data
    int data;
	for(int i = 0; i < m_sizeX * m_sizeY; ++i)
    {
		myfile >> data;
        switch(data)
        {
            case 255:   m_grid.push_back(new Floor(false));
                        break;
            case 155:   m_grid.push_back(new Floor(true));
                        break;
            case 100:   m_grid.push_back(new Wall);
                        break;
            case 0:     m_grid.push_back(new Space);
                        break;
            default :   std::cout << "default" << std::endl;
                        break;
        }
    }

    //int midX = (int)(m_sizeX*0.5);
    //int midY = (int)(m_sizeY*0.5);

    for (int i = 0; i < m_sizeX; i++)
    {
        for (int j = 0; j < m_sizeY; j++)
        {
            // set position with i,j
            m_grid[i*m_sizeX+j]->setPosX(i);
            m_grid[i*m_sizeX+j]->setPosZ(j);
        }
    }

	// close file
	myfile.close();
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
    std::uniform_int_distribution<int> uniformIntXDistrib(0, m_sizeX-1);
    std::uniform_int_distribution<int> uniformIntYDistrib(0, m_sizeY-1);
  
    for (int i = 0; i < nbObstacles; i++)
    {
        do {
            posX = uniformIntXDistrib(generator);
            posY = uniformIntYDistrib(generator);
        } while(!isEmpty(posX, posY)); // can't put obstacle (we want a floor with no obstacle)
        
        m_obstacles.push_back(new Obstacle(glm::vec3(posX, uniformIntXDistrib(generator)%2 * 0.25, posY)));  //TODO: mieux gérer la hauteur des obstacles   
    }
}

void GameMap::initLights(const int nbLights)
{
    //int midX =(int)(m_sizeX*0.5);
    //int midY =(int)(m_sizeY*0.5);

    // select seed from time
    unsigned seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
    // select a generator
    std::mt19937 generator(seed);
    // uniform int distribution
    std::uniform_int_distribution<int> uniformIntXDistrib(0, m_sizeX-1);
    std::uniform_int_distribution<int> uniformIntYDistrib(0, m_sizeY-1);
    // uniform real distribution
    std::uniform_real_distribution<float> uniformRealColorDistrib(0, 1);

  
    for (int i = 0; i < nbLights; i++)
    {
        glm::vec3 pos(uniformIntXDistrib(generator), 0.2, uniformIntYDistrib(generator));
        glm::vec3 color(uniformRealColorDistrib(generator), uniformRealColorDistrib(generator), uniformRealColorDistrib(generator));

        m_lights.push_back(new Light(pos, color));    
    }
}

bool GameMap::isEmpty(const int posX, const int posZ) const
{
    //int midX = (int)(m_sizeX*0.5);
    //int midY = (int)(m_sizeY*0.5);

    // if there is already an obstacle
    for (auto it = m_obstacles.begin(); it != m_obstacles.end(); it++)
    {
        if((*it)->getPosX() == posX && (*it)->getPosZ() == posZ)
            return false;
    }
    return m_grid[posX*m_sizeX+posZ]->possibleAdd;
}

bool GameMap::onAngle(const glm::vec3 pos) const
{ 
    return m_grid[(int)pos.x*m_sizeX + (int)pos.z]->canTurn; //test if player is on a turn case
};

void GameMap::drawGameMap(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos) const
{
    //draw path
    for (auto it = m_grid.begin(); it != m_grid.end(); it++)
        (*it)->draw(view, projection, model, camPos, m_sceneLight, m_lights);

    //draw obstacles
    for (auto it = m_obstacles.begin(); it != m_obstacles.end(); it++)
        (*it)->draw(view, projection, model, camPos, m_sceneLight, m_lights);

    //draw lights
    for (auto it = m_lights.begin(); it != m_lights.end(); it++)
        (*it)->draw(view, projection, model, 0.2f);
}
