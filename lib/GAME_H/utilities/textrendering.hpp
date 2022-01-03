#pragma once
#ifndef TEXTRENDERING_H
#define TEXTRENDERING_H

#include "shader_m.hpp"
#include <iostream>
#include <map>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};




class TextRendering
{
private:
    Shader m_shader;
    unsigned int m_VAO, m_VBO;
    std::map<GLchar, Character> Characters;
public:
    void initTextRendering(float window_width, float window_height);
    int initFreeType();
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
};

#endif