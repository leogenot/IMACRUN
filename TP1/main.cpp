#include "App.hpp"
using namespace std;
//hello test commit a deux
static App& get_app(GLFWwindow* window)
{
    return *reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
}
// settings
const unsigned int window_width  = 1280;
const unsigned int window_height = 720;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float  lastX      = (float)window_width / 2.0;
float  lastY      = (float)window_height / 2.0;
bool   firstMouse = true;

int main(int argc, char** argv)
{
    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
#ifdef __APPLE__
    /* We need to explicitly ask for a 3.3 context on Mac */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "IMACRUN", nullptr, nullptr);
    if (!window) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    Shader skyboxShader("TP1/shaders/shader.vs", "TP1/shaders/shader.fs");

    vector<string> faces{
        "TP1/ressources/skybox/right.jpg",
        "TP1/ressources/skybox/left.jpg",
        "TP1/ressources/skybox/top.jpg",
        "TP1/ressources/skybox/bottom.jpg",
        "TP1/ressources/skybox/front.jpg",
        "TP1/ressources/skybox/back.jpg"};

    /* Create the App */
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    App app{w, h, faces, skyboxShader, camera};

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        app.render(window, camera, w, h, skyboxShader);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
