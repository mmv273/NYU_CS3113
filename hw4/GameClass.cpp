
#include "GameClass.h"
#include "entity.h"
GameClass::GameClass() {
	Init();
	done = false;
	state = STATE_MAIN_MENU;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	fontTexture = LoadTexture("font1.png");

}

void DrawText(const GLuint &fontTexture,
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


GLuint GameClass::LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_FreeSurface(surface);
	return textureID;
}
void GameClass::Init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	
	player = new Entity(0.0f, 0.1f, 0.06f, 0.06f);
	coin = new Entity(.8f, .8f, 0.06f, 0.06f);
    stat.push_back(new Entity(0.0f, 0.0f, 0.25f, 0.1f));
	stat.push_back(new Entity(-0.8f, -0.3f, 0.25f, 0.1f));
	stat.push_back(new Entity(0.8f, -0.3f, 0.25f, 0.1f));
	stat.push_back(new Entity(-0.8f, 0.3f, 0.25f, 0.1f));
	stat.push_back(new Entity(0.8f, 0.3f, 0.25f, 0.1f));
	stat.push_back(new Entity(0.0f, -0.7f, 0.25f, 0.1f));
	stat.push_back(new Entity(0.0f, 0.7f, 0.25f, 0.1f));
	stat.push_back(new Entity(1.0f, 1.0f, 3.0f, 0.002f)); //top border
	stat.push_back(new Entity(-1.33f, -1.0f, .002f, 3.0f));// left border
	stat.push_back(new Entity(1.33f, 1.0f, .002f, 3.0f)); //right border
	stat.push_back(new Entity(-1.0f, -1.0f, 3.0f, 0.002f));// bottom border
}
GameClass::~GameClass() {
	delete player;
	SDL_Quit();
}
void GameClass::Render() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// render stuff
	switch (state) {
	case STATE_MAIN_MENU:
		DrawText(fontTexture, "Welcome to The Platformer", -1.1f, 0.25f, 0.09f, 0.005f, 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case STATE_GAME_LEVEL:
		player->render();
		coin->render();
		for (size_t i = 0; i < stat.size(); i++){
			stat[i]->render();
		}
		break;
	case STATE_WINNER:
		DrawText(fontTexture, "WINNER WINNER", -0.9f, 0.3f, 0.09f, 0.005f, 1.0f, 1.0f, 1.0f, 1.0f);
		DrawText(fontTexture, "Press esc to quit.", -0.8f, -0.4f, 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	}
	SDL_GL_SwapWindow(displayWindow);

}
void GameClass::Update(float elapsed) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				player->jump();
			}
		}
	}
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
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
		Update(elapsed);
		Render();
		
	}
	timeLeftOver = fixedElapsed;
	

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	
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
	else if (state == STATE_GAME_LEVEL ){
		if (player->collidesWith(coin)){
			state = STATE_WINNER;
		}
	}
	else if (state == STATE_WINNER){
		if (keys[SDL_SCANCODE_ESCAPE]){
			done = true;
		}
	}
	
	return done;

}

void GameClass::FixedUpdate(){
	player->movement();
	player->FixedUpdate(stat, coin);
}