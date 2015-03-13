

#include "GameClass.h"

// 60 FPS (1.0f/60.0f)



GameClass::GameClass() {
	Init();
	done = false;
	state = STATE_MAIN_MENU;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

}
void GameClass::Init() {
	//video stuff
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);


	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	GLuint mario = LoadTexture("mario.png");
	GLuint block = LoadTexture("block.png");
	GLuint coin1 = LoadTexture("coin.png");
	player = new Entity(0.0f, 0.1f, 0.06f, 0.06f, mario);
	coin = new Entity(.8f, .8f, 0.06f, 0.06f,coin1);
    stat.push_back(new Entity(0.0f, 0.0f, 0.25f, 0.1f, block));
	stat.push_back(new Entity(-0.8f, -0.3f, 0.25f, 0.1f, block));
	stat.push_back(new Entity(0.8f, -0.3f, 0.25f, 0.1f, block));
	stat.push_back(new Entity(-0.8f, 0.3f, 0.25f, 0.1f, block));
	stat.push_back(new Entity(0.8f, 0.3f, 0.25f, 0.1f, block));
	stat.push_back(new Entity(0.0f, -0.7f, 0.25f, 0.1f, block));
	stat.push_back(new Entity(0.0f, 0.7f, 0.25f, 0.1f, block));
	stat.push_back(new Entity(1.0f, 1.0f, 3.0f, 0.002f,block)); //top border
	stat.push_back(new Entity(-1.33f, -1.0f, .002f, 3.0f, block));// left border
	stat.push_back(new Entity(1.33f, 1.0f, .002f, 3.0f, block)); //right border
	stat.push_back(new Entity(-1.0f, -1.0f, 3.0f, 0.002f, block));
}
GameClass::~GameClass() {
	delete player;
	SDL_Quit();
}
void GameClass::Render() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	GLuint fontTexture = LoadTexture("font1.png");
	// render stuff
	switch (state) {
	case STATE_MAIN_MENU:
		DrawText(fontTexture, "Welcome to The Platformer", -1.1f, 0.25f, 0.09f, 0.005f, 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case STATE_GAME_LEVEL:
		player->Render();
		coin->Render();
		for (size_t i = 0; i < stat.size(); i++){
			stat[i]->Render();
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
bool GameClass::UpdateAndRender() {
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