#pragma once
#include "entity.h"

Entity::Entity(float x, float y, float width, float height)
	:x(x), y(y), width(width), height(height) {
	collidedBottom = false;
	//texture= LoadTexture(image_path);


	//if (isStatic){

	//}
	//else {
	velocity_x = 0.20f;
	velocity_y = 0.9f;
	acceleration_x = 0.0f;
	acceleration_y = 0.0f;
	gravity = -.900f;
	friction_x = 0.59f;
	friction_y = 0.20f;
	scale = .0001f;
	//}
}
Entity::Entity(){};

void Entity::Render(){

	//DrawSprite(texture, x, y, width, height, scale);
	DrawRectangle(x, y, width, height);
}

bool Entity::collidesWith(Entity *entity){
	if (x + (width) > (entity->x - ((entity->width))) &&
		(x - (width)) < (entity->x + (entity->width)) &&
		(y + (height)) > (entity->y - (entity->height)) &&
		(y - (height)) < (entity->y + (entity->height)))
	{
		return true;
	}
	return false;
}
float Entity::lerp(float v0, float v1, float t){
	return (1.0 - t)* v0 + t*v1;

}

void Entity::jump(){
	if (collidedBottom){
		velocity_y = 1.0f;
		collidedBottom = false;
	}

}
void Entity::FixedUpdate(vector <Entity*> stat, Entity* coin){

	//if (collidesWith(coin)){
		

	//}
	

	velocity_y = lerp(velocity_y, 0.0, friction_y * FIXED_TIMESTEP);
	velocity_y += acceleration_y * FIXED_TIMESTEP;
	velocity_y += gravity * FIXED_TIMESTEP;
	y += velocity_y*FIXED_TIMESTEP;
	for (size_t i = 0; i < stat.size(); i++){
		if (collidesWith(stat[i])){
			float yPen = fabs(fabs(y - stat[i]->y) - height - stat[i]->height);
			if (y > stat[i]->y){
				y += yPen + 0.0001;
				velocity_y = 0;
				collidedBottom = true;
			}
			else {
				y -= yPen + 0.0001;
			}
		}
	}
		velocity_x = lerp(velocity_x, 0.0, friction_x * FIXED_TIMESTEP);
		velocity_x += acceleration_x * FIXED_TIMESTEP;
		x += velocity_x * FIXED_TIMESTEP;
		for (size_t j = 0; j < stat.size(); j++){


			if (collidesWith(stat[j])){
				float xPen = fabs(fabs(x - stat[j]->x) - width - stat[j]->width);
				if (x > stat[j]->x){
					x += xPen + 0.0001;
				}
				else {
					x -= xPen + 0.0001;
				}
			}
		}
}
void Entity::movement(){
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_LEFT]) {
		acceleration_x = -2.0;
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {

		acceleration_x = 2.0;
	}
	
	else {
		acceleration_x = 0;
	}
	
}