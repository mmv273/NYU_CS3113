

#include "GameClass.h"

// 60 FPS (1.0f/60.0f)



GameClass::GameClass() {
	Init();
	done = false;
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

	//other stuff
	//textImg = LoadTexture("font1.png");
	player = new Entity(0.0f, 0.1f, 0.06f, 0.06f);
    block = new Entity(0.0f, -0.2f, 0.2f, 0.2f);
}
GameClass::~GameClass() {
	delete player;
	SDL_Quit();
}
void GameClass::Render() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// render stuff
	player->Render();
	block->Render();
	//DrawText(textImg, "Hello World", 0.2f, -0.1f, -0.9f, 0.5f, 1.0, 1.0, 1.0, 1.0);
	//DrawRectangle(0.0f, -0.2f, 0.2f, 0.2f);

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
	}
	timeLeftOver = fixedElapsed;
	Update(elapsed);




	Update(elapsed);
	Render();
	return done;
}

void GameClass::FixedUpdate(){
	//prints once per second
	//static int counter = 0;
	//if(!(counter % 60)) printf("a");
	//counter++;

	player->movement();
	player->FixedUpdate(block);
}