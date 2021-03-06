/*
Mani Vivek
Homework 2: PONG
If player one wins the ball changes to blue while if player two wins the ball stays white. 
While the game progresses the spped of the ball gets faster to make the game harder
*/

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>


const float TOP = 1.0f;
const float BOTTOM = -1.0f;
const float LEFT = -1.33f;
const float RIGHT = 1.33f;

class Entity {
public:


	Entity(float x, float y, float w, float h, float s, char *image_Path)
	{
		// load texture function
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

	float getX() { 
		return posx; }
	float getY() {
		return posy; }
	float getSpeed() { 
		return speed; }
	float getDirectionX() { 
		return direction_X; }
	float getDirectionY() { 
		return direction_Y; }
	void setX(float newX) { 
		posx = newX; }
	void setY(float newY) { 
		posy = newY; }
	void setDirectionX(float newDirectionX) { 
		direction_X = newDirectionX; }
	void setDirectionY(float newDirectionY) { 
		direction_Y = newDirectionY; }

	//draw sprites function
	void draw(){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(posx, posy, 0.0);
		glRotatef(0.0, 0.0, 0.0, 1.0);
		GLfloat quad[] = { -0.5f * width, 0.5f * height, -0.5f * width, -0.5f * height, 0.5f *width, -0.5f * height, 0.5f * width, 0.5f * height };
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
	//function to change the ball image to a different image if player 1 wins 
	void reloadtexture(char *image_Path){
		SDL_Surface *surface = IMG_Load(image_Path);
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		SDL_FreeSurface(surface);
		texture = textureID;
	}
	// used set the ball in motion to start the game
	void move(){
		setDirectionX(0.09);
		setDirectionY(-0.09);
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


bool ProcessEvents(SDL_Event &event, const Uint8 *keys, Entity *first, Entity *second, float &elapsed) {
	// our SDL event loop
	// check input events
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
	// player one movement were W means up and S means down
	}
	if (keys[SDL_SCANCODE_W] && first->getY() < 0.8) {
		first->setY(first->getY() + elapsed);
	}
	else if (keys[SDL_SCANCODE_S] && first->getY() > -0.8) {
		first->setY(first->getY() - elapsed);
	}
	// player two movement were UP means up and down means down 
	if (keys[SDL_SCANCODE_UP] && second->getY() < .8) {
		second->setY(second->getY() + elapsed);
	}
	else if (keys[SDL_SCANCODE_DOWN] && second->getY() > -0.8) {
		second->setY(second->getY() - elapsed);
	}
	return true;
}

void update(Entity *first, Entity *second, Entity *ball, bool &firstwon, float &elapsed) {

	// finds the balls speed and direction
	ball->setX(ball->getX() + (elapsed * (ball->getDirectionX() * ball->getSpeed())));
	ball->setY(ball->getY() + (elapsed * (ball->getDirectionY() * ball->getSpeed())));

	// checks if the ball hits the top of the window
	if (ball->getY() > TOP) { 
		ball->setDirectionY(-1.0 * ball->getDirectionY());
	}//checks if the ball hits the bottom of the window
	else if (ball->getY() < BOTTOM) {
		ball->setDirectionY(-1.0 * ball->getDirectionY());
	}
	// checks if the ball went through player two paddle thus giving the point to player one
	if (ball->getX() > RIGHT) { 
		firstwon = true;
		ball->setX(0.0);
		ball->setY(0.0);
		ball->setDirectionX(-1.0 * ball->getDirectionX());
	}
	// checks if the ball went through player one paddle thus giving the point to player two
	else if (ball->getX() < LEFT) { 
		firstwon = false;
		ball->setX(0.0);
		ball->setY(0.0);
		ball->setDirectionX(-1.0 * ball->getDirectionX());
	}

	// checks if the ball collidied with player one paddleand slowly speeds up the ball to increase the diffuculity 
	if (ball->getX() < -1.2 &&
		ball->getY() > first->getY() - 0.2 &&
		ball->getY() < first->getY() + 0.2)
	{
		ball->setDirectionX(-1.0 * ball->getDirectionX() + .005);
	}
	// checks if the ball collided with player two paddle and slowly speeds up the ball to increase diffuculity 
	else if (ball->getX() > 1.2 &&
		ball->getY() > second->getY() - 0.2 &&
		ball->getY() < second->getY() + 0.2)
	{
		ball->setDirectionX(-1.0 * ball->getDirectionX() - .005);
	}

	
}

void render( Entity *first, Entity *second, Entity *ball, bool &firstwon, float &elapsed) {

	// starts the screen with black 
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT);
	first->draw();
	second->draw();
	ball->draw();
	// if player one wins the ball turns blue
	if (firstwon == true) { 
		ball->reloadtexture("blueball.png");
		first->draw();
		second->draw();
		ball->draw();
	}
	// if the player two wins the ball stays white
	if (firstwon == false) { 
		ball->reloadtexture("ball.png");
		first->draw();
		second->draw();
		ball->draw();
	}

	SDL_GL_SwapWindow(displayWindow);

}

void cleanUp() { SDL_Quit(); }


int main(int argc, char *argv[])
{
	SDL_Event event;
	setup();
	float lastFrameTicks = 0.0;
	float ticks = (float)SDL_GetTicks() / 1000.0;
	float elapsed = ticks - lastFrameTicks;
	bool firstwon;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	Entity *first = new Entity(-1.3f, 0.0f, 0.1f, 0.3f, 1.0f, "paddle.png");
	Entity *second = new Entity(1.3, 0.0, 0.1, 0.3, 1.0, "paddle.png");
	Entity *ball = new Entity(0.0, 0.0, 0.1, 0.1, 6.0, "ball.png");
	ball->move();
	while (ProcessEvents(event, keys, first, second, elapsed)) {
		ticks = (float)SDL_GetTicks() / 1000.0;
		elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		update(first, second, ball, firstwon, elapsed);
		render(first, second, ball, firstwon, elapsed);
	}
	cleanUp();
	return 0;
}
