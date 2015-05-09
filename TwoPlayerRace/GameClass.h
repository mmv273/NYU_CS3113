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
//#include <SDL_mixer.h>
using namespace std;

enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_PLAYER1, STATE_PLAYER2 };
class GameClass {
public:
	GameClass();
	~GameClass();
	void Init();
	bool processEvents();
	void Render();
	void Update(float elapsed);
	GLuint LoadTexture(const char *image_path);
	void DrawText(const GLuint &fontTexture,
		const std::string &text,
		const float &x,
		const float &y,
		const float &size,
		const float &spacing,
		const float &r,
		const float &g,
		const float &b,
		const float &a);
	void readTileMap();
	bool readHeader(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	bool readEntityData(std::ifstream &stream);
	void RenderTiles();
	void getTileCoordinates(float tileX, float tileY, int *gridX, int *gridY);
	float mapCollisionX(float x, float y);
	float mapCollisionY(float x, float y);
	void entityCollisionX(Entity* entity);
	void entityCollisionY(Entity* entity);
	void placeEntity(string type, float placeX, float placeY);
	bool winner();
	void renderLevel();
	void FixedUpdate();
	Entity* player1;
	Entity* player2;
	Entity* bomb1;
	Entity* bomb2;
	Entity* bomb3;
	Entity* bomb4;
	Entity* bomb5;
	Entity* bomb6;
	Entity* bomb7;
	Entity* pink1;
	Entity* pink2;
	Entity* pink3;
	Entity* pink4;
	Entity* pink5;
	Entity* pink6;
	Entity* pink7;
	Entity* star1;
	Entity* star2;
	Entity* star3;
	Entity* goal;
	//Mix_Music *music;
	//Mix_Chunk *someSound;
	bool hit1;
	bool hit2;
	bool hit3;
	vector <Entity*> stars;
	vector <Entity*> bombs;
	vector <Entity*> pinks;
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;
	SDL_Event event;
	GLuint spriteSheet;
	int state;
	GLuint fontTexture;
	float translateX;
	float translateY;
	int mapWidth;
	int mapHeight;
	unsigned char **levelData;
};