#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>
#include <vector>

using namespace std;

void DrawRectangle(float x, float y, float r1, float r2);

GLuint LoadTexture(const char *image_path);


void DrawText(int fontTexture, string text, float size, float spacing, float x, float y, float r, float g, float b, float a);

void DrawSprite(GLint texture, float x, float y, float width, float height, float scale);