#include "GAME_H/floor.hpp"
#include "GAME_H/utilityFunction.hpp"

Floor::Floor()
{
    possibleAdd = true;
    Shader shader("GAME/shaders/objet3D.vs", "GAME/shaders/multipleLights.fs");
    m_shader = shader;
    setPosY(0);
    m_texture = loadTexture<const char>("assets/textures/floor/brickwall.jpg");

    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         0.5f, -0.25f,  0.5f, 1.0f, 0.0f, 0.0f, 2.0f, 0.0f,
        -0.5f, -0.25f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.25f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 2.0f,

         0.5f, -0.25f,  0.5f, 1.0f, 0.0f, 0.0f, 2.0f, 0.0f,
        -0.5f, -0.25f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 2.0f,
         0.5f, -0.25f, -0.5f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f};

    // VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    
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

void Floor::draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 camPos, SceneLight sceneLight, std::vector<Light*> lights) const
{
    m_shader.use();
    int i = 0;
    for (auto it = lights.begin(); it != lights.end(); it++)
    {
        const char* variable = "pointLights[";
        const char* attribPos = "].position";
        const char* attribColor = "].color";
        char uniformNamePosition[24];
        char uniformNameColor[21];
        sprintf(uniformNamePosition, "%s%i%s", variable, i, attribPos);
        sprintf(uniformNameColor, "%s%i%s", variable, i, attribColor);
        
        m_shader.setVec3(uniformNamePosition, (*it)->getPos());
        m_shader.setVec3(uniformNameColor, (*it)->getColor());
        i++;
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
    model = glm::translate(model, getPos());
    m_shader.setMat4("model", model);

    glBindVertexArray(m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}