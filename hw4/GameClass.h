#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>


#include "common.h"
#include "entity.h"
using namespace std;

class GameClass {
public:
	GameClass();
	~GameClass();
	void Init();
	bool UpdateAndRender();
	void Render();
	void Update(float elapsed);
	void FixedUpdate();
	Entity* player;
	Entity* block;
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;
	SDL_Event event;
	GLuint textImg;
};