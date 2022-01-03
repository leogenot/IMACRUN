#include "GAME_H/App.hpp"
#include "GLFW/glfw3.h"

App::App(int window_width, int window_height)
{
}

void App::size_callback(int width, int height)
{
    _width  = width;
    _height = height;
}

void App::destroy()
{
    // Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    glfwTerminate();
}