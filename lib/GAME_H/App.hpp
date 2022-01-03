#pragma once
#define GLFW_INCLUDE_NONE
#ifndef APP_H
    #define APP_H

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
    #include "player/camera.hpp"
    
    #include "player/eyeCamera.hpp"
    #include "game.hpp"
    #include "gamemap/gamemap.hpp"
    #include "glad/glad.h"
    #include "imgui/imgui.h"
    #include "imgui/imgui_impl_glfw.h"
    #include "imgui/imgui_impl_opengl3.h"
    #include "utilities/json.hpp"
    #include "lighting/light.hpp"
    #include "3dmodels/model.hpp"
    #include "player/player.hpp"
    #include "utilities/shader_m.hpp"
    #include "gamemap/skybox.hpp"
    #include "utilities/textrendering.hpp"
    #include "player/trackballCamera.hpp"
    
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