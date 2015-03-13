
#include "math.h"
#include "common.h"
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
class Entity {
public:
	Entity(float x, float y, float width, float height);
	Entity();
	void Render();
	bool collidesWith(Entity* entity2);
	void FixedUpdate(vector <Entity*> stat, Entity* coin);
	float lerp(float v0, float v1, float t);
	void movement();
	//SheetSprite sprite;
	float x;
	float y;
	float gravity;
	float width;
	float height;
	float velocity_x;
	float velocity_y;
	float acceleration_x;
	float acceleration_y;
	GLuint texture;
	float friction_x;
	float friction_y;
	float mass;
	bool isStatic;
	void jump();
	bool playerDead();
	bool enableCollisions;
	float scale;

	//bool collidedTop;
	bool collidedBottom;
	//bool collidedLeft;
	//bool collidedRight;
};