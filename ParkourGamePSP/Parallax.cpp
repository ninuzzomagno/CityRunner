#include"Game.h"

ParallaxBackground::ParallaxBackground(std::string pathEnv) {
	std::ifstream FILE(pathEnv+"frames.F", std::ios::in);

	FILE >> this->maxFrame;

	FILE >> this->multi;
	this->text = new OSL_IMAGE * [multi];
	
	if (this->multi == 1) {

		this->frames = new IRect[maxFrame];

		for (int i = 0; i < maxFrame; i++) {
			FILE >> this->frames[i].x;
			FILE >> this->frames[i].y;
			FILE >> this->frames[i].w;
			FILE >> this->frames[i].h;
		}

		std::string pathText = pathEnv + "layers.png";
		this->text[0] = oslLoadImageFilePNG(const_cast<char*>(pathText.c_str()), OSL_IN_VRAM | OSL_SWIZZLED, OSL_PF_8888);
	}
	else {
		std::string name;
		for (int i = 0; i < this->multi; i++) {
			FILE >> name;
			this->text[i] = oslLoadImageFilePNG(const_cast<char*>((pathEnv+name).c_str()), OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
		}
	}

	FILE.close();

	FILE.open(pathEnv + "layers.L", std::ios::in);

	FILE >> this->maxLayers;

	this->layers = new ParallaxLayer*[this->maxLayers];

	int y, j;
	float v;

	for (int i = 0; i < this->maxLayers; i++) {

		FILE >> y;
		FILE >> j;
		FILE >> v;
		this->layers[i] = new ParallaxLayer(y, j, v);

	}
	FILE.close();
}

void ParallaxBackground::update(float dP) {
	for (int i = 0; i < this->maxLayers; i++) {
		this->layers[i]->update(dP);
	}
}

void ParallaxBackground::draw() {
	if (this->multi == 0) {
		for (int i = 0; i < this->maxLayers; i++) {
			oslSetImageTile(this->text[0],this->frames[this->layers[i]->index].x, this->frames[this->layers[i]->index].y, this->frames[this->layers[i]->index].w, this->frames[this->layers[i]->index].h);
			oslDrawImageXY(this->text[0], this->layers[i]->pos[0], this->layers[i]->pos[1]);
			oslDrawImageXY(this->text[0], this->layers[i]->pos[0] + 480, this->layers[i]->pos[1]);
		}
	}
	else {
		for (int i = 0; i < this->maxLayers; i++) {
			oslDrawImageXY(this->text[i], this->layers[i]->pos[0], this->layers[i]->pos[1]);
			oslDrawImageXY(this->text[i], this->layers[i]->pos[0] + 480, this->layers[i]->pos[1]);
		}
	}
}

ParallaxLayer::ParallaxLayer(int py, int index, float vel) {
	this->pos[1] = py;
	this->pos[0] = 0;
	this->vel = vel;
	this->index = index;
}

void ParallaxLayer::update(float dP) {
	this->pos[0] -= dP* this->vel;
	if (this->pos[0] <= -480)
		this->pos[0] = 0;
}

