#include "GAME_H/obstacle.hpp"
#include "GAME_H/utilityFunction.hpp"

Obstacle::Obstacle(glm::vec3 pos, unsigned int texture) : m_pos(pos), m_texture(texture)
{
    Shader shader("GAME/shaders/objet3D.vs", "GAME/shaders/multipleLights.fs");
    m_shader = shader;

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  1.0f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  1.0f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  1.0f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f,  0.0f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f,  0.0f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  1.0f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  1.0f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  1.0f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f,  0.0f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  1.0f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  1.0f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f,  0.0f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.0f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.0f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  1.0f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  1.0f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  1.0f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.0f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.0f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.0f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  1.0f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f,  0.0f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.0f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.0f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.0f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.0f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.0f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  1.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  1.0f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  1.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  1.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  1.0f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  1.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    // VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    
    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Obstacle::draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, SceneLight sceneLight, std::vector<Light*> lights, glm::vec3 playerPos, int renderRadius) const
{
    m_shader.use();
    int i = 0;
    for (auto it = lights.begin(); it != lights.end(); it++)
    {
        if ((*it)->getPos().x < playerPos.x + renderRadius && (*it)->getPos().x > playerPos.x - renderRadius && (*it)->getPos().y < playerPos.y + renderRadius && (*it)->getPos().y > playerPos.y - renderRadius)
        {
            std::string uniformNamePosition = "pointLights[" + std::to_string(i) + "].position";
            std::string uniformNameColor = "pointLights[" + std::to_string(i) + "].color";
            
            m_shader.setVec3(uniformNamePosition,  glm::vec3((*it)->getPos()));
            m_shader.setVec3(uniformNameColor, (*it)->getColor());
            i++;
        }
    }

    //light
    m_shader.setVec3("dirLight",  sceneLight.getDirection());
    m_shader.setVec3("lightColor",  sceneLight.getColor());
    m_shader.setVec3("viewPos",  camPos);

    //material
    m_shader.setInt("material.diffuse", 0);
    //m_shader.setInt("material.specular", 0);
    m_shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    m_shader.setFloat("material.shininess", 32.0f);

    //matrix
    m_shader.setMat4("view", view);
    m_shader.setMat4("projection", projection);
    model = glm::translate(model, m_pos);
    if(m_pos.y == 1)
        model = glm::translate(model, glm::vec3(0, -0.75, 0));
    model = glm::scale(model, glm::vec3(0.4, 0.3, 0.4)); //TODO: gérer si l'obsacle est bien perpendiculaire au chemin
    m_shader.setMat4("model", model);

    glBindVertexArray(m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}