#pragma once
#define GLFW_INCLUDE_NONE
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
#include "gamemap.hpp"
#include "skybox.hpp"
#include "shader_m.h"
#include "trackballCamera.hpp"
#include "eyeCamera.hpp"

#include "cube.hpp"
#include "light.hpp"

#include <map>
#include "ft2build.h"
#include FT_FREETYPE_H
using namespace std;
class App {
public:
    App(int window_width, int window_height);
    //void render(Camera camera);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouse_button_callback(int button, int action, int mods);
    void scroll_callback(double xoffset, double yoffset);
    void cursor_position_callback(double xpos, double ypos);
    void size_callback(int width, int height);
    void destroy();
    bool fixedCamera = true;

    

private:
    int    _width{};
    int    _height{};
    GLuint skyboxVAO, skyboxVBO, planeVAO, planeVBO, cubeVAO, cubeVBO;
};
