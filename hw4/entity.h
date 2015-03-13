#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "math.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
using namespace std;

class Entity {
public:
	Entity(float x, float y, float width, float height);
	Entity();
	GLuint LoadTexture(const char *image_path);
	void DrawText(const GLuint &fontTexture, const std::string &text, const float &x, const float &y, const float &size, const float &spacing, const float &r, const float &g, const float &b, const float &a);
	void DrawRectangle(float x, float y, float r1, float r2);
	void render();
	bool collidesWith(Entity* entity2);
	void FixedUpdate(vector <Entity*> stat, Entity* coin);
	float lerp(float v0, float v1, float t);
	void movement();
	float x;
	float y;
	float gravity;
	float width;
	float height;
	float velocity_x;
	float velocity_y;
	float acceleration_x;
	float acceleration_y;
	GLuint texture;
	float friction_x;
	float friction_y;
	float mass;
	bool isStatic;
	void jump();
	bool playerDead();
	bool enableCollisions;
	float scale;
	bool collidedBottom;
	
};