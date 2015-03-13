#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "entity.h"
using namespace std;

enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_WINNER };
class GameClass {
public:
	GameClass();
	~GameClass();
	void Init();
	bool processEvents();
	void Render();
	void Update(float elapsed);
	GLuint GameClass::LoadTexture(const char *image_path);
	void FixedUpdate();
	Entity* player;
	Entity* coin;
	vector <Entity*> stat;
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;
	SDL_Event event;
	GLuint textImg;
	int state;
	GLuint fontTexture;
};