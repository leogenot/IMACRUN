#include "GAME_H/App.hpp"

#define STB_IMAGE_IMPLEMENTATION

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

static App& get_app(GLFWwindow* window)
{
    return *reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
}

// settings
const unsigned int window_width  = 1280;
const unsigned int window_height = 720;

TrackballCamera trackball_camera;
eyeCamera       eye_camera;
Camera*         camera = &eye_camera;
Player          player(camera);

double lastX       = (double)window_width / 2.0;
double lastY       = (double)window_height / 2.0;
bool   firstMouse  = true;
bool   fixedCamera = false;

bool MouseIn  = false;
bool MouseOut = true;

bool show_main_menu_window    = false;
bool show_quit_window = false;
bool show_options_window = false;

bool paused = false;

// light
glm::vec3  lightDir(-0.8, -1.0, -0.6);
glm::vec3  lightColor(0.7, 0.9, 1.0);
SceneLight sceneLight(lightDir, lightColor);

// gamemap
GameMap       gamemap(sceneLight);
Skybox        skybox;
Cube          cube;
TextRendering textrendering;

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1); // Enable vsync

    Shader boxShader("GAME/shaders/floor.vs", "GAME/shaders/floor.fs");

    Model ourModel("assets/models/flash.obj");
    player.initPlayer();
    // generate gamemap whith file
    gamemap.loadGameMap("assets/map16.pgm");
    int nbObstacles = 10;
    int nbLights    = 5;
    gamemap.initObstacles(nbObstacles);
    gamemap.initLights(nbLights);
    skybox.initSkybox();

    textrendering.initTextRendering(window_width, window_height);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.Fonts->AddFontFromFileTTF("assets/fonts/Antonio-Regular.ttf", 70);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Our state

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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
        processInput(window);
        float currentFrame = (float)glfwGetTime();
        if (!paused) {
            // per-frame time logic
            // --------------------
            
            deltaTime          = currentFrame - lastFrame;
            // render
            // ------
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 model      = glm::mat4(1.0f);
            glm::mat4 view       = player.getCamera()->GetViewMatrix(player.getPos());
            glm::mat4 projection = glm::perspective(eye_camera.Zoom, (float)window_width / (float)window_height, 0.1f, 100.0f);

            if (player.getCamera()->getCameraType() == 0) //no drawing with eye camera
                player.draw(view, projection, model, ourModel);

            gamemap.drawGameMap(view, projection, model, player.getCamera()->getPos());
            skybox.draw(view, projection, model, player.getCamera()->GetViewMatrix(player.getPos()));
            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            textrendering.RenderText("Flash McQueen", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
            textrendering.RenderText("KATCHAAAAW", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        }
        else {
            deltaTime = 0;
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            /* if (show_main_menu_window)
                ImGui::ShowDemoWindow(&show_main_menu_window); */

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            if (show_main_menu_window) {
                static bool             use_work_area = true;
                static ImGuiWindowFlags flags         = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

                // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
                // Based on your use case you may want one of the other.
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
                ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
                ImGui::Begin("Main Menu IMACRUN", &show_main_menu_window, flags);

                if (ImGui::Button("New Game")) // Buttons return true when clicked (most widgets return true when edited/activated)
                    std::cout << "hello" << std::endl;

                if (ImGui::Button("Resume Game")) // Buttons return true when clicked (most widgets return true when edited/activated)
                {
                    show_main_menu_window = false;
                    paused           = !paused;
                }

                if (ImGui::Button("Save Game")) // Buttons return true when clicked (most widgets return true when edited/activated)
                    std::cout << "hello" << std::endl;

                if (ImGui::Button("Load Game")) // Buttons return true when clicked (most widgets return true when edited/activated)
                    std::cout << "hello" << std::endl;

                if (ImGui::Button("Options")) // Buttons return true when clicked (most widgets return true when edited/activated)
                    show_options_window = true;

                if(ImGui::Button("Quit Game"))
                    show_quit_window = true; // Buttons return true when clicked (most widgets return true when edited/activated)

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // Options confirmation window
            if (show_options_window) {
                static bool             use_work_area = true;
                static ImGuiWindowFlags flags         = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

                // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
                // Based on your use case you may want one of the other.
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
                ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

                ImGui::Begin("Game options", &show_options_window, flags);
                if (ImGui::Button("Back to main menu"))
                    show_options_window = false;
                ImGui::End();
            }

            // Exit confirmation window
            if (show_quit_window) {
                static bool             use_work_area = true;
                static ImGuiWindowFlags flags         = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

                // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
                // Based on your use case you may want one of the other.
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
                ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

                ImGui::Begin("Quit Game", &show_quit_window, flags); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Text("Are you sure you want to quit game?");
                if (ImGui::Button("NO stay here"))
                    show_quit_window = false;
                if (ImGui::Button("YES quit game"))
                    glfwSetWindowShouldClose(window, true);
                ImGui::End();
            }
            // Renders the ImGUI elements
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        
        lastFrame          = currentFrame;
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
    //Quit
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Mouse capture
    static int oldStatePause = GLFW_RELEASE;
    int        newStatePause = glfwGetKey(window, GLFW_KEY_ESCAPE);
    if (newStatePause == GLFW_RELEASE && oldStatePause == GLFW_PRESS) {
        if (paused) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            show_main_menu_window = !show_main_menu_window;
            paused           = false;
            std::cout << "not paused" << std::endl;
            
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            paused           = true;
            show_main_menu_window = !show_main_menu_window;
            std::cout << "paused" << std::endl;
            
            
        }
    }
    oldStatePause = newStatePause;

    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.ProcessKeyboard(FORWARD,deltaTime, gamemap);

    if (gamemap.onAngle(player.getPos())) // Rotate player
    {
        static int oldStateRotateLeft = GLFW_RELEASE;
        int        newStateRotateLeft = glfwGetKey(window, GLFW_KEY_A);
        if (newStateRotateLeft == GLFW_RELEASE && oldStateRotateLeft == GLFW_PRESS) {
            player.ProcessKeyboard(ROTATELEFT, deltaTime, gamemap);
        }
        oldStateRotateLeft = newStateRotateLeft;

        static int oldStateRotateRight = GLFW_RELEASE;
        int        newStateRotateRight = glfwGetKey(window, GLFW_KEY_D);
        if (newStateRotateRight == GLFW_RELEASE && oldStateRotateRight == GLFW_PRESS) {
            player.ProcessKeyboard(ROTATERIGHT, deltaTime, gamemap);
        }
        oldStateRotateRight = newStateRotateRight;
    }
    else // Translate player
    {
        //left
        static int oldStateLeft = GLFW_RELEASE;
        int        newStateLeft = glfwGetKey(window, GLFW_KEY_A);
        if (newStateLeft == GLFW_RELEASE && oldStateLeft == GLFW_PRESS) {
            player.ProcessKeyboard(LEFT, deltaTime, gamemap);
        }
        oldStateLeft = newStateLeft;

        //right
        static int oldStateRight = GLFW_RELEASE;
        int        newStateRight = glfwGetKey(window, GLFW_KEY_D);
        if (newStateRight == GLFW_RELEASE && oldStateRight == GLFW_PRESS) {
            player.ProcessKeyboard(RIGHT, deltaTime, gamemap);
        }
        oldStateRight = newStateRight;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        eye_camera.Reset();

    if (!player.onGround && !player.isFalling) //then rising
        player.Rise(deltaTime);
    else if (!player.onGround)
        player.Fall(deltaTime);

    //Jump
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        player.Jump();

    //Switch camera
    static int oldState = GLFW_RELEASE;
    int        newState = glfwGetKey(window, GLFW_KEY_T);
    if (newState == GLFW_RELEASE && oldState == GLFW_PRESS) {
        if (player.getCamera()->getCameraType() == 0) {
            player.setCamera(&eye_camera);
        }
        else {
            player.setCamera(&trackball_camera);
        }
        std::cout << player.getCamera()->getCameraType() << std::endl;
    }
    oldState = newState;

    //Fixed camera
    static int oldStateFixedCam = GLFW_RELEASE;
    int        newStateFixedCam = glfwGetKey(window, GLFW_KEY_L);
    if (newStateFixedCam == GLFW_RELEASE && oldStateFixedCam == GLFW_PRESS) {
        if (fixedCamera) {
            fixedCamera = false;
            firstMouse  = true;
        }
        else
            fixedCamera = true;
    }
    oldStateFixedCam = newStateFixedCam;
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
    if (!fixedCamera) {
        if (firstMouse) {
            lastX      = xpos;
            lastY      = ypos;
            firstMouse = false;
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        player.getCamera()->ProcessMouseMovement((float)xoffset, (float)yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!fixedCamera) {
        player.getCamera()->ProcessMouseScroll((float)yoffset);
    }
}