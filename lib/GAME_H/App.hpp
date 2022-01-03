#pragma once
#define GLFW_INCLUDE_NONE
#ifndef APP_H
    #define APP_H

    #include <stb_image.h>
    #include <chrono>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtc/type_ptr.hpp>
    #include <iostream>
    #include <map>
    #include <string>
    #include <thread>
    #include <vector>
    #include "GLFW/glfw3.h"
    #include "camera.hpp"
    #include "cube.hpp"
    #include "eyeCamera.hpp"
    #include "game.hpp"
    #include "gamemap.hpp"
    #include "glad/glad.h"
    #include "imgui/imgui.h"
    #include "imgui/imgui_impl_glfw.h"
    #include "imgui/imgui_impl_opengl3.h"
    #include "json.hpp"
    #include "light.hpp"
    #include "model.hpp"
    #include "player.hpp"
    #include "shader_m.hpp"
    #include "skybox.hpp"
    #include "textrendering.hpp"
    #include "trackballCamera.hpp"
    
using json = nlohmann::json;

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

#endif