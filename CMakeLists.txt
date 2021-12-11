cmake_minimum_required(VERSION 3.0)

# Set project name
project(IMACRUN C CXX)
# Set C++ version
set(CXX_STANDARD 14)

##
# Boring setup
##
# Set the path to the lib folder
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
# Set the folder where the executable is created
set(EXECUTABLE_OUTPUT_PATH 
    ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}
)

# Glfw
set(GLFW_DIR lib/glfw)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)

# Glad
add_library(GLAD STATIC lib/glad/src/glad.c)


# Build the different TPs
function(setup_game GAME)
    set(TARGET_NAME ${GAME}_exe)
    # Grab all the source files
    file(GLOB_RECURSE MY_SOURCES
        ${GAME}/*
        lib/glimac/*
    )
    # Create target executable
    add_executable(${TARGET_NAME}
        ${MY_SOURCES}
    )

    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD ${CXX_STANDARD})

    # Set include directories
    include_directories(
        ${GAME}
        lib
        lib/glimac
    )

    # OpenGL
    find_package(OpenGL REQUIRED)
    include_directories(
        ${OPENGL_INCLUDE_DIR}
        lib/glad/include
    )
    target_link_libraries(${TARGET_NAME}
        ${OPENGL_LIBRARIES}
        GLAD
        freetype
        tinyobjloader
        assimp
    )

    # GLFW
    include_directories(${GLFW_DIR}/include)
    include_directories(${GLFW_DIR}/deps)
    target_link_libraries(${TARGET_NAME}
        glfw
    )

        # ---Add FreeType---
    add_subdirectory(${LIB_DIR}/freetype)
    target_link_libraries(${TARGET_NAME}
        freetype
    )

    # ---Add tinyobjloader---
    add_subdirectory(${LIB_DIR}/tinyobjloader)
    target_link_libraries(${TARGET_NAME}
    tinyobjloader
    )

        # ---Add assimp---
        add_subdirectory(${LIB_DIR}/assimp)
        target_link_libraries(${TARGET_NAME}
        assimp
        )


    ##
    # Add a post build operation to copy shaders to the output folder (where the executable is created)
    ##
    function(copy_folder_to_the_output_folder _FOLDERPATH)
    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/${_FOLDERPATH}
            $<TARGET_FILE_DIR:${TARGET_NAME}>/${_FOLDERPATH}
    )
    endfunction()
    copy_folder_to_the_output_folder("assets")
    copy_folder_to_the_output_folder(${GAME}/"shaders")
endfunction(setup_game)

setup_game(GAME)