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

	// close file
	myfile.close();
}

void Map::initObstacles(const int nbObstacles)
{
    float posX;
    float posY;
    for (int i = 0; i < nbObstacles; i++)
    {
        do {
            posX = rand() % m_sizeX;
            posY = rand() % m_sizeY;
        } while(!isEmpty(posX, posY));//can't put obstacle (we want a floor with no obstacle)
        
        m_obstacles.push_back(new Obstacle(posX, posY, rand()%2));    
    }
}

bool Map::isEmpty(float posX, float posY)
{
    // if there is already an obstacle
    for (auto it = m_obstacles.begin(); it != m_obstacles.end(); it++)
    {
        if((*it)->getPosX() == posX && (*it)->getPosY() == posY)
            return false;
    }
    return m_grid[posX*m_sizeX+posY]->possibleAdd;
}

void Map::drawMap(glm::mat4 view, glm::mat4 projection, glm::mat4 model)
{
    //draw path
    for (int i = 0; i < m_sizeX; i++)
    {
        for (int j = 0; j < m_sizeY; j++)
        {
            // draw each element at position i,j
            m_grid[i*m_sizeX+j]->draw(view, projection, model, i, j);
        }
    }

    //draw obstacles
    for (auto it = m_obstacles.begin(); it != m_obstacles.end(); it++)
        (*it)->draw(view, projection, model);
}
