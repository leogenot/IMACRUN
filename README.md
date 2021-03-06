# IMACRUN Setup

**it is easy to use on all platforms** (Linux, Windows and Mac).

## Compiling

You need to install [CMake](https://cmake.org/download/).

To use CMake I recommend this VS Code extension : [ms-vscode.cmake-tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools). You will need to setup the extension with a compiler. Here is [the tutorial](https://code.visualstudio.com/docs/cpp/cmake-linux). It is based on Linux but at the bottom of the page you will find the explanations to adapt it for [Windows](https://code.visualstudio.com/docs/cpp/config-msvc) and [Mac](https://code.visualstudio.com/docs/cpp/config-clang-mac).

Alternatively you can just create a *build* folder at the root of this project, open a terminal and run `cmake ..` ; chances are it will detect what compiler you have installed and generate the appropriate Makefile / Visual Studio solution / Xcode project.

## main.cpp

In here we initialize [*glfw*](https://www.glfw.org/docs/latest/) (the library that handles the window and user inputs), create a window and then start the app. You don't really need to modify this file, except to change your window name or initial size.

## assets

The *assets* folder will be copied to the folder where your executable is created. You can put all the assets that your project needs in there, like textures, shaders, 3D models *etc.*
