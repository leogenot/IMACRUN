
#ifndef SKYBOX_H
#define SKYBOX_H
#include "element.hpp"
#include "shader_m.hpp"
#include "../stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "GLFW/glfw3.h"
#include "camera.hpp"
#include "glad/glad.h"


#include "gamemap.hpp"
#include "trackballCamera.hpp"

class Skybox
{
private:
    Shader m_skyboxShader;
    unsigned int m_texture;
    unsigned int m_VAO, m_VBO;
    unsigned int cubemapTexture;
public:
    void initSkybox();
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::mat4 cameraViewMatrix) const;
    unsigned int loadCubemap(std::vector<std::string> faces);
};

#endif