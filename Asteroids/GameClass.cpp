//Mani Vivek

#include "GameClass.h"

#define SHEET_SPRITE_COLUMNS 16
#define SHEET_SPRITE_ROWS 8
#define TILE_SIZE 0.08f


float GameClass::randomRange(float min, float max){
	float random = ((float)rand()) / (float)RAND_MAX;
	return (random*(max - min)) + min;
}
GameClass::GameClass() {
	Init();
	
	done = false;
	state = STATE_MAIN_MENU;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	fontTexture = LoadTexture("font1.png");
	spriteSheet = LoadTexture("sprites.png");
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("background.mp3");
	Mix_PlayMusic(music, -1); // repeat it indefinitely
	someSound = Mix_LoadWAV("app.wav");
	keys = SDL_GetKeyboardState(NULL);
	Matrix init;
	glLoadMatrixf(init.ml);
	pCollision = false;
	aCollision = false;
	createEntities();
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

void GameClass::createEntities(){
	player = new Entity(0.0, 0.0, 80, 16, 8, .75, 0.0, 0.0, 0.0, spriteSheet);
	
	for (int i = 0; i < 5; i++) {
		asteroids.push_back(new Entity(randomRange(-0.5, 0.5), randomRange(-0.75, 0.75), 63, 16, 8,  randomRange(0.5, 1.0), randomRange(0.0, 2.25), randomRange(-0.25, 0.25), randomRange(-0.25, 0.25), spriteSheet));
	}

}

void GameClass::DrawText(string text, float x, float y, float size, float spacing, float r, float g, float b, float a) {
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

	glPushMatrix();
	glTranslatef(x, y, 0.0);

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
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
	Mix_FreeMusic(music);
	Mix_FreeChunk(someSound);
	SDL_Quit();
}
bool GameClass::checkCollision(Entity* a, Entity* b) {
	if (a == b) {
		return false;
	}
	a->generateMatrix();
	b->generateMatrix();

	Matrix aInverse = a->matrix.inverse();
	Matrix bInverse = b->matrix.inverse();

	float aWidth = a->width * a->scale * 0.9f;
	float aHeight = a->height * a->scale * 0.9f;
	float bWidth = b->width * b->scale * 0.9f;
	float bHeight = b->height * b->scale * 0.9f;

	Vector aTL = Vector(-aWidth, aHeight);
	Vector aBL = Vector(-aWidth, -aHeight);
	Vector aBR = Vector(aWidth, -aHeight);
	Vector aTR = Vector(aWidth, aHeight);

	aTL = a->matrix * aTL;
	aBL = a->matrix * aBL;
	aBR = a->matrix * aBR;
	aTR = a->matrix * aTR;

	aTL = bInverse * aTL;
	aBL = bInverse * aBL;
	aBR = bInverse * aBR;
	aTR = bInverse * aTR;

	float minX, maxX, minY, maxY;
	minX = min(min(min(aTL.x, aBL.x), aTR.x), aBR.x);
	maxX = max(max(max(aTL.x, aBL.x), aTR.x), aBR.x);
	if (!(minX <= bWidth && maxX >= -bWidth)) {
		return false;
	}
	minY = min(min(min(aTL.y, aBL.y), aTR.y), aBR.y);
	maxY = max(max(max(aTL.y, aBL.y), aTR.y), aBR.y);
	if (!(minY <= bHeight && maxY >= -bHeight)) {
		return false;
	}

	Vector bTL = Vector(-bWidth, bHeight);
	Vector bBL = Vector(-bWidth, -bHeight);
	Vector bBR = Vector(bWidth, -bHeight);
	Vector bTR = Vector(bWidth, bHeight);

	bTL = b->matrix * bTL;
	bBL = b->matrix * bBL;
	bTR = b->matrix * bTR;
	bBR = b->matrix * bBR;

	bTL = aInverse * bTL;
	bBL = aInverse * bBL;
	bBR = aInverse * bBR;
	bTR = aInverse * bTR;

	minX = min(min(min(bTL.x, bBL.x), bTR.x), bBR.x);
	maxX = max(max(max(bTL.x, bBL.x), bTR.x), bBR.x);
	if (!(minX <= aWidth && maxX >= -aWidth)) {
		return false;
	}
	minY = min(min(min(bTL.y, bBL.y), bTR.y), bBR.y);
	maxY = max(max(max(bTL.y, bBL.y), bTR.y), bBR.y);
	if (!(minY <= aHeight && maxY >= -aHeight)) {
		return false;
	}

	return true;
}

void GameClass::Update(float elapsed) {
	
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			
			}
		if (keys[SDL_SCANCODE_UP]) {
			player->x += 10.8f * player->vector.x * elapsed;
			player->y += 10.8f * player->vector.y * elapsed;
		}
		else if (keys[SDL_SCANCODE_DOWN]) {
			player->x -= 10.8f * player->vector.x * elapsed;
			player->y -= 10.8f * player->vector.y * elapsed;
		}
		if (keys[SDL_SCANCODE_RIGHT]) {
			player->rotation -= 30.0f * elapsed;
		}
		else if (keys[SDL_SCANCODE_LEFT]) {
			player->rotation += 30.0f * elapsed;
		}
		else if (keys[SDL_SCANCODE_Q]){
			done = true;
		}
	}
}



void GameClass::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	// render stuff
	switch (state) {
	case STATE_MAIN_MENU:
		DrawText("Welcome to ASTEROIDS", -1.1f, 0.25f, 0.09f, 0.005f, 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case STATE_GAME_LEVEL:
		RenderLevel();
		break;
	case STATE_GAME_OVER:
		DrawText( "GAME OVER", -0.9f, 0.3f, 0.09f, 0.005f, 1.0f, 1.0f, 1.0f, 1.0f);
		DrawText("Press esc to quit.", -0.8f, -0.4f, 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	}
	SDL_GL_SwapWindow(displayWindow);



}
bool GameClass::processEvents() {

	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	player->generateMatrix();
	player->setVector();
	
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		Render();
		Update(elapsed);
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;
	//if the state is main menu press space to start the game
	if (state == STATE_MAIN_MENU){
		if (keys[SDL_SCANCODE_SPACE]){
			state = STATE_GAME_LEVEL;
		}
		// if you want to quit press esc
		else if (keys[SDL_SCANCODE_ESCAPE]){
			done = true;
		}
	}
	else if (state == STATE_GAME_LEVEL){
		if (ticks > 10.0){
			for (size_t j = 0; j < asteroids.size(); j++) {
				if (checkCollision(player, asteroids[j])) {
					state = STATE_GAME_OVER;
				}
			}
		}
	}
	else if (state == STATE_GAME_OVER){
		if (keys[SDL_SCANCODE_ESCAPE]){
			done = true;
		}
	}

	return done;

}


void GameClass::FixedUpdate() {
	player->FixedUpdate();
	pCollision = false;
	aCollision = false;
	if (player->x <= -1.4f || player->x >= 1.4f) {
		player->x = -0.99f;
	}
	if (player->y <= -1.0f || player->y >= 1.0f) {
		player->y *= -0.99f;
	}

	for (size_t i = 0; i < asteroids.size(); i++) {
		
		asteroids[i]->FixedUpdate();

		if (asteroids[i]->x <= -1.4f || asteroids[i]->x >= 1.4f) {
			asteroids[i]->x *= -0.99f;
		}
		if (asteroids[i]->y <= -1.0f || asteroids[i]->y >= 1.0f) {
			asteroids[i]->y *= -0.99f;
		}
		for (size_t j = 0; j < asteroids.size(); j++) {
			if (checkCollision(asteroids[i], asteroids[j])) {
				aCollision = true;
				Vector distance = Vector(asteroids[i]->x - asteroids[j]->x, asteroids[i]->y - asteroids[j]->y);
				float distLength = distance.length();
				distance.normalize();

				asteroids[i]->x += distance.x * 0.02f;
				asteroids[i]->y += distance.y * 0.02f;

				asteroids[j]->x -= distance.x * 0.02f;
				asteroids[j]->y -= distance.y * 0.02f;
			}
		}

	}
}

float lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}

void GameClass::RenderLevel() {
	glClear(GL_COLOR_BUFFER_BIT);
	player->Draw();
	for (size_t i = 0; i < asteroids.size(); i++) {
		asteroids[i]->Draw();
	}
}


