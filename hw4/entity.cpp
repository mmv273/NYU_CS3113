#pragma once
#include "entity.h"

Entity::Entity(float x, float y, float width, float height)
	:x(x), y(y), width(width), height(height) {
	collidedBottom = false;
	
	velocity_x = 0.20f;
	velocity_y = 0.9f;
	acceleration_x = 0.0f;
	acceleration_y = 0.0f;
	gravity = -.900f;
	friction_x = 0.59f;
	friction_y = 0.20f;
	scale = 1.0f;
	
}
Entity::Entity(){};

GLuint Entity::LoadTexture(const char *image_path) {
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

void Entity::DrawRectangle(float x, float y, float r1, float r2){
	GLfloat quad[] = { (x - r1), (y + r2),
		(x - r1), (y - r2),
		(x + r1), (y - r2),
		(x + r1), (y + r2) };

	glLoadIdentity();
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_QUADS, 0, 4);
	//glEnd();

}

void Entity::DrawText(const GLuint &fontTexture,
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



void Entity::render(){
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