#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>
#include "Vector.h"
#include "Matrix.h"
#include "math.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
using namespace std;

class Entity {
public:
	Entity(float x,float y,int index, int col, float rows,float scale, float rotation,float velocityX, float velcityY,GLuint t);
	Entity();
	void Draw();
	float lerp(float v0, float v1, float t);
	void setVector();
	void generateMatrix();
	void FixedUpdate();
	void Render();
	float x;
	float y;
	float u;
	float v;
	float width;
	float height;
	float scale;
	float rotation;
	float speed;
	float velocityX;
	float velocityY;
	Matrix matrix;
	Vector vector;
	GLuint t;
};