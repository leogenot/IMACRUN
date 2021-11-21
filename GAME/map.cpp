#include "GAME_H/map.hpp"

void Map::loadMap(const std::string &path)
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
            case 255:   m_grid.push_back(new Floor);
                        break;
            case 100:   m_grid.push_back(new Wall);
                        break;
            case 0:     m_grid.push_back(new Space);
                        break;
            default :   std::cout << "default" << std::endl;
                        break;
        }
    }

    float midX = m_sizeX*0.5;
    float midY = m_sizeY*0.5;

    for (int i = 0; i < m_sizeX; i++)
    {
        for (int j = 0; j < m_sizeY; j++)
        {
            // set position with i,j
            m_grid[i*m_sizeX+j]->setPosX(i-midX);
            m_grid[i*m_sizeX+j]->setPosZ(j-midY);
        }
    }

	// close file
	myfile.close();
}

void Map::initObstacles(const int nbObstacles)
{
    float posX;
    float posY;
    float midX = m_sizeX*0.5;
    float midY = m_sizeY*0.5;

    for (int i = 0; i < nbObstacles; i++)
    {
        do {
            posX = rand() % m_sizeX;
            posY = rand() % m_sizeY;
        } while(!isEmpty(posX, posY)); // can't put obstacle (we want a floor with no obstacle)
        
        m_obstacles.push_back(new Obstacle(glm::vec3(posX - midX, rand()%2 * 0.25 - 0.2, posY - midY)));  //TODO: mieux gérer la hauteur des obstacles   
    }
}

void Map::initLights(const int nbLights)
{
    float midX = m_sizeX*0.5;
    float midY = m_sizeY*0.5;

    for (int i = 0; i < nbLights; i++)
    {
        glm::vec3 pos(rand() % m_sizeX - midX, 0.2, rand() % m_sizeY - midY);
        glm::vec3 color(rand() % 100 * 0.01, rand() % 100 * 0.01, rand() % 100 * 0.01);

        m_lights.push_back(new Light(pos, color));    
    }
}

bool Map::isEmpty(const int posX, const int posZ) const
{
    float midX = m_sizeX*0.5;
    float midY = m_sizeY*0.5;

    // if there is already an obstacle
    for (auto it = m_obstacles.begin(); it != m_obstacles.end(); it++)
    {
        if((*it)->getPosX() == posX - midX && (*it)->getPosZ() == posZ - midY)
            return false;
    }
    return m_grid[posX*m_sizeX+posZ]->possibleAdd;
}

void Map::drawMap(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos) const
{
    //draw path
    for (auto it = m_grid.begin(); it != m_grid.end(); it++)
        (*it)->draw(view, projection, model, camPos, m_sceneLight, m_lights);

    //draw obstacles
    for (auto it = m_obstacles.begin(); it != m_obstacles.end(); it++)
        (*it)->draw(view, projection, model, camPos, m_sceneLight, m_lights);

    //draw lights
    for (auto it = m_lights.begin(); it != m_lights.end(); it++)
        (*it)->draw(view, projection, model, 0.2);
}
