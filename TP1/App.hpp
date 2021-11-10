#pragma once
#define GLFW_INCLUDE_NONE
#include <vector>
#include <string>
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <camera.h>
#include <shader_m.h>


#include "glimac/FilePath.hpp"
#include "glimac/Program.hpp"

#include <iostream>
using namespace std;

class App {
public:
    App(int window_width, int window_height,vector<string> faces, Shader skyboxShader, Camera camera);
    void render(GLFWwindow *window, Camera camera, int window_width, int window_height, Shader skyboxShader);
    void key_callback(int key, int scancode, int action, int mods);
    void mouse_button_callback(int button, int action, int mods);
    void scroll_callback(double xoffset, double yoffset);
    void cursor_position_callback(double xpos, double ypos);
    void size_callback(int width, int height);
    void processInput(GLFWwindow *window, Camera camera);
    unsigned int loadCubemap(vector<string> faces);

private:
    int _width{};
    int _height{};
    GLuint skyboxVAO, skyboxVBO;
};