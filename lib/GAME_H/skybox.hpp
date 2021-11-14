
#ifndef SKYBOX_H
#define SKYBOX_H
#include "element.hpp"
#include "shader_m.h"
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "GLFW/glfw3.h"
#include "camera.h"
#include "glad/glad.h"
#include "glimac/FilePath.hpp"
#include "glimac/Program.hpp"
#include "map.hpp"
#include "shader_m.h"
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
    void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model,Camera camera) const;
    unsigned int loadCubemap(std::vector<std::string> faces);
};

#endif