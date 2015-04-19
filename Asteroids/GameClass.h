#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "entity.h"
#include <SDL_mixer.h>
#include <math.h>
#include <algorithm>
using namespace std;

enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
class GameClass {
public:
	GameClass();
	~GameClass();
	void Init();
	void createEntities();
	bool collisons();
	bool processEvents();
	void Render();
	void Update(float elapsed);
	GLuint LoadTexture(const char *image_path);
	void DrawText(string text, float x, float y, float size, float spacing, float r, float g, float b, float a);
	bool checkCollision(Entity* a, Entity* b);
	void RenderLevel();
	void FixedUpdate();
	Entity* player;
	vector<Entity*> asteroids;
	Mix_Music *music;
	Mix_Chunk *someSound;
	bool done;
	bool pCollision;
	bool aCollision;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;
	SDL_Event event;
	GLuint spriteSheet;
	int state;
	GLuint fontTexture;
	const Uint8 *keys;
	float randomRange(float min, float max);
};