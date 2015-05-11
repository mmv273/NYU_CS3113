//Mani Vivek
//Collect the three items and you win :D
#include "GameClass.h"
#include "entity.h"

#define SHEET_SPRITE_COLUMNS 16
#define SHEET_SPRITE_ROWS 8
#define TILE_SIZE 0.08f
GameClass::GameClass() {
	Init();
	done = false;
	state = STATE_MAIN_MENU;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	fontTexture = LoadTexture("font1.png");
	spriteSheet = LoadTexture("sprites.png");
	playerOneScore = 0;
	playerTwoScore = 0;
	
	//Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	//music = Mix_LoadMUS("background.mp3");
	//Mix_PlayMusic(music, -1); // repeat it indefinitely
	
	//someSound = Mix_LoadWAV("app.wav");
	
}

void GameClass::Init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

}

void GameClass::DrawText(const GLuint &fontTexture,
	const std::string &text,
	const float &x,
	const float &y,
	const float &size,
	const float &spacing,
	const float &r,
	const float &g,
	const float &b,
	const float &a) {
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float texture_size = 1.0 / 16.0;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	std::vector<float> colorData;

	for (size_t i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		vertexData.insert(vertexData.end(),
		{ ((size + spacing) * i) + (-0.5f * size),
		0.5f * size,
		((size + spacing) * i) + (-0.5f * size),
		-0.5f * size,
		((size + spacing) * i) + (0.5f * size),
		-0.5f * size,
		((size + spacing) * i) + (0.5f * size),
		0.5f * size });
		texCoordData.insert(texCoordData.end(),
		{ texture_x,
		texture_y,
		texture_x,
		texture_y + texture_size,
		texture_x + texture_size,
		texture_y + texture_size,
		texture_x + texture_size,
		texture_y });
	}
	glLoadIdentity();
	glTranslatef(x, y, 0.0);

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

GLuint GameClass::LoadTexture(const char *imagePath) {
	// load the texture
	SDL_Surface *surface = IMG_Load(imagePath);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		4,
		surface->w,
		surface->h,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_FreeSurface(surface);
	return textureID; 
}

GameClass::~GameClass() {
	//Mix_FreeMusic(music);
	//Mix_FreeChunk(someSound);
	SDL_Quit();
}

void GameClass::readTileMap(char *map){
	ifstream infile(map);
	string line;
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[Object Layer 1]") {
			readEntityData(infile);
		}
	}
}

bool GameClass::readHeader(std::ifstream &stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while
		(getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}
	if
		(mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { // allocate our map data
		levelData = new unsigned char*[mapHeight];
		for
			(int i =
			0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}}bool GameClass::readLayerData(std::ifstream &stream) {
	std::string line;
	while (std::getline(stream, line)) {
		if (line == "") { break; }
		std::istringstream sStream(line);
		std::string key, value;
		std::getline(sStream, key, '=');
		std::getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				std::getline(stream, line);
				std::istringstream lineStream(line);
				std::string tile;
				for (int x = 0; x < mapWidth; x++) {
					std::getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 12;
					}
				}
			}
		}
	}
	return true;
}

bool GameClass::readEntityData(std::ifstream &stream) {
	std::string line, type;
	while (std::getline(stream, line)) {
		if (line == "") { break; }
		std::istringstream sStream(line);
		std::string key, value;
		std::getline(sStream, key, '=');
		std::getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			std::istringstream lineStream(value);
			std::string xPosition, yPosition;
			std::getline(lineStream, xPosition, ',');
			std::getline(lineStream, yPosition, ',');
			float placeX = atoi(xPosition.c_str()) / 16 * 0.08f + 0.08f;
			float placeY = atoi(yPosition.c_str()) / 16 * - 0.08f;
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}
void GameClass::placeEntity(string type, float placeX, float placeY){	if (type == "player1"){		player1 = new Entity(placeX, placeY, 98, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);	}	if (type == "player2"){		player2 = new Entity(placeX, placeY, 80, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);	}	if (type == "bomb1"){		bomb1 = new Entity(placeX, placeY, 75, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		bombs.push_back(bomb1);	}	if (type == "bomb2"){		bomb2 = new Entity(placeX, placeY, 75, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		bombs.push_back(bomb2);	}	if (type == "bomb3"){		bomb3 = new Entity(placeX, placeY, 75, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		bombs.push_back(bomb3);	}	if (type == "bomb4"){		bomb4 = new Entity(placeX, placeY, 75, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		bombs.push_back(bomb4);	}	if (type == "bomb5"){		bomb5 = new Entity(placeX, placeY, 75, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		bombs.push_back(bomb5);	}	if (type == "bomb6"){		bomb6 = new Entity(placeX, placeY, 75, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		bombs.push_back(bomb6);	}	if (type == "bomb7"){		bomb7 = new Entity(placeX, placeY, 75, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		bombs.push_back(bomb7);	}	if (type == "pink1"){		pink1 = new Entity(placeX, placeY, 28, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		pinks.push_back(pink1);	}	if (type == "pink2"){		pink2 = new Entity(placeX, placeY, 28, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		pinks.push_back(pink2);	}	if (type == "pink3"){		pink3 = new Entity(placeX, placeY, 28, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		pinks.push_back(pink3);	}	if (type == "pink4"){		pink4 = new Entity(placeX, placeY, 28, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		pinks.push_back(pink4);	}	if (type == "pink5"){		pink5 = new Entity(placeX, placeY, 28, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		pinks.push_back(pink5);	}	if (type == "pink6"){		pink6 = new Entity(placeX, placeY, 28, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		pinks.push_back(pink6);	}	if (type == "pink7"){		pink7 = new Entity(placeX, placeY, 28, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		pinks.push_back(pink7);	}	if (type == "star1"){		star1 = new Entity(placeX, placeY, 49, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		stars.push_back(star1);	}	if (type == "star2"){		star2 = new Entity(placeX, placeY, 49, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		stars.push_back(star2);	}	if (type == "star3"){		star3 = new Entity(placeX, placeY, 49, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);		stars.push_back(star3);	}	if (type == "goal"){		goal = new Entity(placeX, placeY, 22, SHEET_SPRITE_COLUMNS, SHEET_SPRITE_ROWS, 0.5, spriteSheet);	}}
void GameClass::Update(float elapsed) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
				player1->jump();
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_W) {
				player2->jump();
			}
		}
	}

}



void GameClass::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	// render stuff
	switch (state) {
	case STATE_MAIN_MENU:
		DrawText(fontTexture, "Welcome to The Race", -1.1f, 0.25f, 0.09f, 0.005f, 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case STATE_GAME_LEVEL1:
		renderLevel();
		glClearColor(0, 0, 255, 0.7);
		break;
	case STATE_GAME_LEVEL2:
		renderLevel();
		glClearColor(0, 0, 255, 0.7);
		break;
	case STATE_GAME_LEVEL3:
		renderLevel();
		glClearColor(0, 0, 0, 0);
		break;
	case STATE_GAME_OVER:
		glClear(GL_COLOR_BUFFER_BIT);
		if (playerOneScore > playerTwoScore){
			DrawText(fontTexture, "PLAYER 1 HAS WON", -0.9f, 0.3f, 0.09f, 0.001f, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (playerOneScore < playerTwoScore){
			DrawText(fontTexture, "PLAYER 2 HAS WON", -0.9f, 0.3f, 0.09f, 0.001f, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		DrawText(fontTexture, "Press esc to quit.", -0.8f, -0.4f, 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case STATE_PLAYER2:
		
		DrawText(fontTexture, "Press esc to quit.", -0.8f, -0.4f, 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	}
	SDL_GL_SwapWindow(displayWindow);



}
bool GameClass::processEvents() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;


	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		if (state == STATE_GAME_LEVEL1 || state == STATE_GAME_LEVEL2|| state== STATE_GAME_LEVEL3) {
			cout << "P1" << playerOneScore << endl;
			if (player1->collidesWith(goal) || player2->y < -1.70f){
				
				playerOneScore += 1;
				nextLevel();

			}
			if (player2->collidesWith(goal) || player1->y < -1.70f){
				playerTwoScore += 1;
				nextLevel();
			}
			
			if (bomb != 0) {
				if (stunTime < 120)  {
					stunTime++;
				}
				else {
					stunTime = 0;
					bomb = 0;
				}
			}
			if (pink != 0) {
				if (slowTime < 120)  {
					slowTime++;
				}
				else {
					slowTime = 0;
					pink = 0;
				}
			}
			if (star != 0) {
				if (boostTime < 120)  {
					boostTime++;
				}
				else {
					boostTime = 0;
					star = 0;
				}
			}
			for (size_t i = 0; i < bombs.size(); i++) {
				if (player1->collidesWith(bombs[i])) {
					bomb = 1;
					bombs.erase(bombs.begin() + i);
				}
				else if (player2->collidesWith(bombs[i])) {
					bomb = 2;
					bombs.erase(bombs.begin() + i);
				}
			}
			for (size_t i = 0; i < pinks.size(); i++) {
				if (player1->collidesWith(pinks[i])) {
					pink = 1;
					pinks.erase(pinks.begin() + i);
				}
				else if (player2->collidesWith(pinks[i])) {
					pink = 2;
					pinks.erase(pinks.begin() + i);
				}
			}
			for (size_t i = 0; i < stars.size(); i++) {
				if (player1->collidesWith(stars[i])) {
					star = 1;
					stars.erase(stars.begin() + i);
				}
				else if (player2->collidesWith(stars[i])) {
					star = 2;
					stars.erase(stars.begin() + i);
				}
			}
			player1->collidedTop = false;
			player1->collidedBottom = false;
			player1->collidedLeft = false;
			player1->collidedRight = false;
			player2->collidedTop = false;
			player2->collidedBottom = false;
			player2->collidedLeft = false;
			player2->collidedRight = false;
			FixedUpdate();
			entityCollisionY(player1);
			entityCollisionX(player1);
			entityCollisionY(player2);
			entityCollisionX(player2);
			
		}
		fixedElapsed -= FIXED_TIMESTEP;
		Update(elapsed);
		Render();
	}
	timeLeftOver = fixedElapsed;
	

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	
	//if the state is main menu press space to start the game
	if (state == STATE_MAIN_MENU){
		if (keys[SDL_SCANCODE_SPACE]){
			nextLevel();
			}
		// if you want to quit press esc
		else if (keys[SDL_SCANCODE_ESCAPE]){
			done = true;
		}
	}
	else if (state == STATE_GAME_LEVEL1 ){
		if (keys[SDL_SCANCODE_ESCAPE]){
			done = true;
		}
		
	}
	else if (state == STATE_GAME_LEVEL2){
		if (keys[SDL_SCANCODE_ESCAPE]){
			done = true;
		}
	}
	else if (state == STATE_GAME_LEVEL3){
		if (keys[SDL_SCANCODE_ESCAPE]){
			done = true;
		}
	}
	else if (state == STATE_GAME_OVER) {
		if (keys[SDL_SCANCODE_ESCAPE]) {
			return false;
		}
	}
	else if (state == STATE_PLAYER2) {
		if (keys[SDL_SCANCODE_ESCAPE]) {
			return false;
		}
	}
	return done;

} 

void GameClass::nextLevel(){
	if (state == STATE_MAIN_MENU){
		stars.clear();
		bombs.clear();
		pinks.clear();
		readTileMap("mymap5.txt");
		state = STATE_GAME_LEVEL1;
		
	}
		
	else if (state == STATE_GAME_LEVEL1){
		stars.clear();
		bombs.clear();
		pinks.clear();
		readTileMap("mymap6.txt");
		state = STATE_GAME_LEVEL2;
		
	}
	else if (state == STATE_GAME_LEVEL2){
		stars.clear();
		bombs.clear();
		pinks.clear();
		readTileMap("mymap7.txt");
		state = STATE_GAME_LEVEL3;

	}
	else if (state == STATE_GAME_LEVEL3){
		
			state = STATE_GAME_OVER;
		
		
	}
}

float lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}


void GameClass::FixedUpdate(){
	player1->movement();
	player2->movement2();
	if (star == 1){
		player1->velocity_y += player1->gravity * FIXED_TIMESTEP;
		player1->velocity_x = lerp(player1->velocity_x, 0.0f, FIXED_TIMESTEP * player1->friction_x);
		player1->velocity_y = lerp(player1->velocity_y, 0.0f, FIXED_TIMESTEP * player1->friction_y);
		player1->velocity_x += player1->acceleration_x * FIXED_TIMESTEP * 1.5;
		player1->velocity_y += player1->acceleration_y * FIXED_TIMESTEP * 1.5;
		player1->x += player1->velocity_x * FIXED_TIMESTEP;
		player1->y += player1->velocity_y * FIXED_TIMESTEP;
	}
	if (pink == 1){
		player1->velocity_y += player1->gravity * FIXED_TIMESTEP;
		player1->velocity_x = 0;
		player1->velocity_y = 0;
		player1->x += player1->velocity_x * FIXED_TIMESTEP;
		player1->y += player1->velocity_y * FIXED_TIMESTEP;
	}
	if (bomb == 1){
		player1->velocity_x = 0;
		player1->velocity_y = 0;
		player1->acceleration_x = 0;
		player1->acceleration_y = 0;
	}
	if (star == 2){
		player2->velocity_y += player2->gravity * FIXED_TIMESTEP;
		player2->velocity_x = lerp(player2->velocity_x, 0.0f, FIXED_TIMESTEP * player2->friction_x);
		player2->velocity_y = lerp(player2->velocity_y, 0.0f, FIXED_TIMESTEP * player2->friction_y);
		player2->velocity_x += player2->acceleration_x * FIXED_TIMESTEP * 1.5;
		player2->velocity_y += player2->acceleration_y * FIXED_TIMESTEP * 1.5;
		player2->x += player2->velocity_x * FIXED_TIMESTEP;
		player2->y += player2->velocity_y * FIXED_TIMESTEP;
	}
	if (pink == 2){
		player2->velocity_y += player2->gravity * FIXED_TIMESTEP;
		player2->velocity_x = 0;
		player2->velocity_y = 0;
		player2->x += player2->velocity_x * FIXED_TIMESTEP;
		player2->y += player2->velocity_y * FIXED_TIMESTEP;
	}
	if (bomb == 2){
		player2->velocity_x = 0;
		player2->velocity_y = 0;
		player2->acceleration_x = 0;
		player2->acceleration_y = 0;
	}
	player1->velocity_y += player1->gravity * FIXED_TIMESTEP;
	player1->velocity_x = lerp(player1->velocity_x, 0.0f, FIXED_TIMESTEP * player1->friction_x);
	player1->velocity_y = lerp(player1->velocity_y, 0.0f, FIXED_TIMESTEP * player1->friction_y);
	player1->velocity_x += player1->acceleration_x * FIXED_TIMESTEP;
	player1->velocity_y += player1->acceleration_y * FIXED_TIMESTEP;
	player1->x += player1->velocity_x * FIXED_TIMESTEP;
	player1->y += player1->velocity_y * FIXED_TIMESTEP;
	
	player2->velocity_y += player2->gravity * FIXED_TIMESTEP;
	player2->velocity_x = lerp(player2->velocity_x, 0.0f, FIXED_TIMESTEP * player2->friction_x);
	player2->velocity_y = lerp(player2->velocity_y, 0.0f, FIXED_TIMESTEP * player2->friction_y);
	player2->velocity_x += player2->acceleration_x * FIXED_TIMESTEP;
	player2->velocity_y += player2->acceleration_y * FIXED_TIMESTEP;
	player2->x += player2->velocity_x * FIXED_TIMESTEP;
	player2->y += player2->velocity_y * FIXED_TIMESTEP;
	
}

void GameClass::getTileCoordinates(float tileX, float tileY, int *gridX, int *gridY) {
	*gridX = (int)(tileX / TILE_SIZE);
	*gridY = (int)(-tileY / TILE_SIZE);
}

float GameClass::mapCollisionX(float x, float y){
	int gridX, gridY;
	getTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > mapWidth || gridY < 0 || gridY > mapHeight) {
		return 0.0f;
	}

	if (levelData[gridY][gridX] == 3 || levelData[gridY][gridX] == 16 || levelData[gridY][gridX] == 33 || levelData[gridY][gridX] == 96 || levelData[gridY][gridX] == 97 || levelData[gridY][gridX] == 32 ){
		float xCoordinate = (gridX * TILE_SIZE); // -(TILE_SIZE * 1.0f);
		return -x - xCoordinate;
	}
	return 0.0f;
}

float GameClass::mapCollisionY(float x, float y){
	int gridX, gridY;
	getTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > mapWidth || gridY < 0 || gridY > mapHeight){
		return 0.0f;
	}

	if (levelData[gridY][gridX] == 3 || levelData[gridY][gridX] == 16 || levelData[gridY][gridX] == 33 || levelData[gridY][gridX] == 96 || levelData[gridY][gridX] == 97 || levelData[gridY][gridX] == 32){
		float yCoordinate = (gridY * TILE_SIZE); // -(TILE_SIZE * 1.0);
		return -y - yCoordinate;
	}
	return 0.0f;
}

void GameClass::entityCollisionX(Entity* entity){
	//check left
	float buffer = mapCollisionX(entity->x - entity->width * 0.5, entity->y);
	if (buffer != 0.0f){
		entity->x -= buffer * FIXED_TIMESTEP * TILE_SIZE;
		entity->velocity_x = 0.0f;
		entity->collidedLeft = true;
	}
	//check right
	buffer = mapCollisionX(entity->x + entity->width*0.5, entity->y);
	if (buffer != 0.0f){
		entity->x += buffer * FIXED_TIMESTEP * TILE_SIZE;
		entity->velocity_x = 0.0f;
		entity->collidedRight = true;
	}
}

void GameClass::entityCollisionY(Entity* entity){
	//check bottom
	float buffer = mapCollisionY(entity->x, entity->y - entity->height*0.5);
	if (buffer != 0.0f){
		entity->y += buffer;
		entity->velocity_y = 0.0f;
		
		entity->collidedBottom = true;
	}
	//check top
	buffer = mapCollisionY(entity->x, entity->y + entity->height*0.5);
	if (buffer != 0.0f){
		entity->y += buffer - TILE_SIZE;
		entity->velocity_y = 0.0f;
		entity->collidedTop = true;
	}
}

void GameClass::RenderTiles() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spriteSheet);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			float u = (float)((int)levelData[y][x] % SHEET_SPRITE_COLUMNS) / (float)SHEET_SPRITE_COLUMNS;
			float v = (float)((int)levelData[y][x] / SHEET_SPRITE_COLUMNS) / (float)SHEET_SPRITE_ROWS;
			float spriteWidth = 1.0 / (float)SHEET_SPRITE_COLUMNS;
			float spriteHeight = 1.0 / (float)SHEET_SPRITE_ROWS;
			vertexData.insert(vertexData.end(), {
				TILE_SIZE * x, -TILE_SIZE * y,
				TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
				(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
				(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
			});
			texCoordData.insert(texCoordData.end(), {
				u, v,
				u, v + spriteHeight,
				u + spriteWidth, v + spriteHeight,
				u + spriteWidth, v
			});
		}
	}
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, mapHeight * mapWidth * 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

void GameClass::renderLevel(){

	glLoadIdentity();
	if (player1->x > player2->x){
		translateX = (-player1->x);
		
	}
	else if (player1->x < player2->x){
		translateX = (-player2->x);
		
	}
	//translateY = (-player1->y + -player2->y) / 2;
	if (state == STATE_GAME_LEVEL3){ translateY = 1.00; }
	else { translateY = 0.75; }
	if (translateX > -1.33f) translateX = -1.33f;
	else if (translateX < -mapWidth*TILE_SIZE + 1.33) translateX = -mapWidth*TILE_SIZE + 1.33;
	/*if (translateY < 1.0f) translateY = 1.0f;
	else if (translateY > -mapWidth*TILE_SIZE -1.0) translateX = mapWidth*TILE_SIZE - 1.0;*/
	glTranslatef(translateX, translateY, 0.0);

	// draw the world
	RenderTiles();

	// draw the players
	player1->Draw(translateX, translateY);
	//coin->Draw(translateX, translateY);
	/*door->Draw(translateX, translateY);
	key->Draw(translateX, translateY);*/
	player2->Draw(translateX, translateY);
	goal->Draw(translateX, translateY);
	for (int i = 0; i < bombs.size(); i++){
		bombs[i]->Draw(translateX,translateY);
	}
	for (int j = 0; j < stars.size(); j++){
		stars[j]->Draw(translateX, translateY);
	}
	for (int k = 0; k < pinks.size(); k++){
		pinks[k]->Draw(translateX, translateY);
	}
}