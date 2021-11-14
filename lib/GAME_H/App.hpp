#pragma once
#define GLFW_INCLUDE_NONE
#include "trackballCamera.hpp"
#include "camera.h"
#include "map.hpp"
#include "shader_m.h"
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glimac/FilePath.hpp"
#include "glimac/Program.hpp"
using namespace std;
class App {
public:
    App(int window_width, int window_height);
    void         render(Camera camera);
    void         key_callback(int key, int scancode, int action, int mods);
    void         mouse_button_callback(int button, int action, int mods);
    void         scroll_callback(double xoffset, double yoffset);
    void         cursor_position_callback(double xpos, double ypos);
    void         size_callback(int width, int height);
    void         destroy();

private:
    int    _width{};
    int    _height{};
    GLuint skyboxVAO, skyboxVBO, planeVAO, planeVBO, cubeVAO, cubeVBO;
    
};
