
#include "entity.h"

Entity::Entity(float x, float y, int index, int col, float rows, float scale, float rotation, float velocityX, float velocityY, GLuint t)
	:x(x), y(y), scale(scale), rotation(rotation),velocityX(velocityX), velocityY(velocityY), t(t) {
	
	u = (float)(index % col) / (float) col;
	v = (float)(index / col) / (float) rows;
	width = (1.0 / col);
	height = (1.0 / rows);
	speed = 1.0f;
	
}
Entity::Entity(){};


void Entity::Draw() {
	generateMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, t);

	GLfloat quad[] = { width * -1.0f * scale,
		height * scale,
		width * -1.0f * scale,
		height * -1.0f * scale,
		width * scale,
		height * -1.0f * scale,
		width * scale,
		height * scale };

	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
}

float Entity::lerp(float v0, float v1, float t){
	return (1.0 - t)* v0 + t*v1;

}

void Entity::setVector(){
	vector = Vector(float(cos(rotation + M_PI / 2.0f)), float(sin(rotation + M_PI / 2.0f)), 0.0f);
}

void Entity::generateMatrix() {
	Matrix scaled;
	scaled.identity();
	scaled.m[0][0] = scale;
	scaled.m[1][1] = scale;

	Matrix rotated;
	rotated.identity();
	rotated.m[0][0] = cos(rotation);
	rotated.m[0][1] = sin(rotation);
	rotated.m[1][0] = -sin(rotation);
	rotated.m[1][1] = cos(rotation);

	Matrix translated;
	translated.identity();
	translated.m[3][0] = x;
	translated.m[3][1] = y;

	matrix.identity();
	matrix = scaled * rotated * translated;
}


void Entity::FixedUpdate() {

	setVector();
	vector.x *= speed;
	vector.y *= speed;
	generateMatrix();

	x += velocityX * FIXED_TIMESTEP;
	y += velocityY * FIXED_TIMESTEP;

	if (x <= -2.0f || x >= 2.0f) {
		x = x * 0.99f;
	}
	if (y <= -2.0f || y >= 2.0f) {
		y = y* -0.99f;
	}
}

