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
            case 0:     m_grid.push_back(new Space);
                        break;
            default :   std::cout << "default" << std::endl;
                        break;
        }
    }

	// close file
	myfile.close();
}

void Map::drawMap(glm::mat4 view, glm::mat4 projection, glm::mat4 model)
{
    for (int i = 0; i < m_sizeX; i++)
    {
        for (int j = 0; j < m_sizeY; j++)
        {
            // draw each element at position i,j
            m_grid[i*m_sizeX+j]->draw(view, projection, model, i, j);
        }
    }
}
