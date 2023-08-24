#include"Game.h"

Cubes::Cubes() {
	for (int i = 0; i < 2; i++) {
		this->pos[i] = 0;
		this->size[i] = 0;
	}
}

Cubes::Cubes(float* p, int* s) {
	for (int i = 0; i < 2; i++) {
		this->pos[i] = p[i];
		this->size[i] = s[i];
	}
}

void Cubes::update(float dP) {
	this->pos[0] -= dP;
	if (this->pos[0] < -this->size[0]) {
		this->pos[0] = oslRandf(720, 480);
		this->size[0] = oslRandf(200, 100);
		this->size[1] = oslRandf(200, 50);
		this->pos[1] = 272 - this->size[1];
	}
}

void Cubes::draw(float vel) {
	if (this->pos[0] > -this->size[0] && this->pos[0] < 480)
		oslDrawFillRect(this->pos[0] + vel, this->pos[1], this->pos[0] + this->size[0] + vel, this->pos[1] + this->size[1], COLOR::black);
}

bool Cubes::collide(Cubes*c) {
	return (this->pos[0]<c->pos[0] + c->size[0] &&
		this->pos[0] + this->size[0] > c->pos[0] &&
		this->pos[1]<c->pos[1] + c->size[1] &&
		this->pos[1] + this->size[1] > c->pos[1]
		);
}