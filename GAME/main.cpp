#include "GAME_H/App.hpp"
void         framebuffer_size_callback(GLFWwindow* window, int width, int height);
void         mouse_callback(GLFWwindow* window, double xpos, double ypos);
void         scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void         processInput(GLFWwindow* window);

static App& get_app(GLFWwindow* window)
{
    return *reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
}

// settings
const unsigned int window_width  = 1280;
const unsigned int window_height = 720;

// camera
Camera          camera(glm::vec3(0.0f, 0.0f, 3.0f));
TrackballCamera Trackcamera;
float           lastX      = (float)window_width / 2.0;
float           lastY      = (float)window_height / 2.0;
bool            firstMouse = true;

// map
Map map;
Skybox skybox;
Cube cube;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
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
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "IMACRUN", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    Shader boxShader("GAME/shaders/floor.vs", "GAME/shaders/floor.fs");

    // generate map whith file
    map.loadMap("assets/map.pgm");
    skybox.initSkybox();
    cube.initCube();
    


    

    /* Create the App */
    int w, h;
    glfwGetWindowSize(window, &w, &h);

    App app{w, h};
/* Hook user inputs to the App */
    glfwSetWindowUserPointer(window, reinterpret_cast<void*>(&app));
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        get_app(window).key_callback(window, key, scancode, action, mods);
    });
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        app.render(camera);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view       = camera.GetViewMatrix();
        //glm::mat4 view       = Trackcamera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)window_width / (float)window_height, 0.1f, 100.0f);



        // floor
        map.drawMap(view, projection, model);
        cube.draw(view, projection, model,1.0f,1.0f, 0.5f);
        
        skybox.draw(view,projection, model, camera);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    app.destroy();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        //Trackcamera.rotateUp(deltaTime);
    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        //Trackcamera.rotateLeft(deltaTime);
    camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    /* if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        if(fixedCamera)
        {
            fixedCamera = false;
            firstMouse = true;
        }
        else
            fixedCamera = true;
    } */
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    /* if(!fixedCamera)
    {
        if (firstMouse) {
            lastX      = xpos;
            lastY      = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        //camera.ProcessMouseMovement(xoffset, yoffset);
        Trackcamera.ProcessMouseMovement(xoffset, yoffset);
    } */

    if (firstMouse) {
        lastX      = xpos;
        lastY      = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    /* if (!fixedCamera) {
        //camera.ProcessMouseScroll(yoffset);
        Trackcamera.moveFront(yoffset);
    } */

    camera.ProcessMouseScroll(yoffset);
}
