#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>
#include <vector>

using namespace std;

void DrawRectangle(float x, float y, float r1, float r2);

GLuint LoadTexture(const char *image_path);


void DrawText(const GLuint &fontTexture,const std::string &text, const float &x, const float &y, const float &size, const float &spacing, const float &r, const float &g, const float &b,const float &a);

void DrawSprite(GLint texture, float x, float y, float width, float height, float scale);