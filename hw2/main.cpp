
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "math.h"



class Entity {
public:


	Entity(const float &x, const float &y, const float &w, const float &h, const float &s, const char *image_Path)
	{
		SDL_Surface *surface = IMG_Load(image_Path);
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		SDL_FreeSurface(surface);
		posx = x;
		posy = y;
		width = w;
		height = h;
		speed = s;
		direction_X = 0.0;
		direction_Y = 0.0;
		texture = textureID;
	}

	float getX() { return posx; }
	float getY() {return posy; }
	float getSpeed() { return speed; }
	float getDirectionX() { return direction_X; }
	float getDirectionY() { return direction_Y; }

	
	void setX(float newX) { posx = newX; }
	void setY(float newY) { posy = newY; }
	void setDirectionX(float newDirectionX) { direction_X = newDirectionX; }
	void setDirectionY(float newDirectionY) { direction_Y = newDirectionY; }

	
	void draw(){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
		glTranslatef(posx, posy, 0.0);
		glRotatef(0.0, 0.0, 0.0, 1.0);

		GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}

	
private:
	float posx;
	float posy;
	int texture;
	float width;
	float height;
	float speed;
	float direction_X;
	float direction_Y;
};


SDL_Window* displayWindow;

void setup() {
	// setup SDL
	// setup OpenGL
	// Set our projection matrix 	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("PONG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}


bool ProcessEvents(SDL_Event &event, const Uint8 *keys, Entity *first, Entity *second, const float &elapsed) {
	// our SDL event loop
	// check input events
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) { // end the game because they closed the window
			return false;
		}
	}
	if (keys[SDL_SCANCODE_W] && first->getY() < 0.5) {
		first->setY(first->getY() + elapsed);
	}
	else if (keys[SDL_SCANCODE_S] && first->getY() > -0.5) {
		first->setY(first->getY() - elapsed);
	}
	if (keys[SDL_SCANCODE_UP] && second->getY() < 0.5) {
		second->setY(second->getY() + elapsed);
	}
	else if (keys[SDL_SCANCODE_DOWN] && second->getY() > -0.5) {
		second->setY(second->getY() - elapsed);
	}
	return true;
}

void render(Entity *first, Entity *second, Entity *ball, bool &firstwon, const float &timeElapsed) {

	
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT);
	first->draw();
	second->draw();
	ball->draw();

	if (firstwon == TRUE) { 
		glClearColor(0.5, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		playerOne->draw();
		playerTwo->draw();
		ball->draw();
	}
	if (playerWin == 2) { // player one scored, set the screen blue to keep track
		glClearColor(0.0, 0.0, 0.5, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		playerOne->draw();
		playerTwo->draw();
		ball->draw();
	}

	SDL_GL_SwapWindow(displayWindow);

}

void cleanUp() { SDL_Quit(); }


int main(int argc, char *argv[])
{

	

	bool done = false;

	SDL_Event event;


	
	float lastFrameTicks = 0.0f;
	float rotate = 0.0f;
	
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		rotate += 100.0f * elapsed;

		glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();


	
		
		
		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}