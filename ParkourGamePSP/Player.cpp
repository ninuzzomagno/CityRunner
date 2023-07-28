#include"Game.h"

Player::Player(int* pos,bool menu) : Cubes(){
	this->pos[0] = pos[0];
	this->pos[1] = pos[1];
	this->vY = 0;
	this->counter = 0.f;
	this->jumperCounter = 0;
	this->ap = new AnimationPlayer("resource/player/animations/player.AP");
	this->fall = false;
	this->end = false;
	if (menu)
		ap->setCurrentAnimation(0);
	this->texture = oslLoadImageFilePNG(const_cast<char*>("resource/player/texture.png"), OSL_IN_RAM, OSL_PF_8888);;
}

bool Player::update(float dt, Cubes** c) {
	this->currentFrame = this->ap->update(dt);
	this->size[0] = this->currentFrame->w;
	this->size[1] = this->currentFrame->h;

	this->g = 10.f;

	bool collision = false;

	for (int i = 0; i < 5; i++) {
		if (this->collide(c[i])) {
			this->pos[0] -= 1;
			if (this->collide(c[i])) {
				collision = true;
				this->g = 0.f;
				this->vY = 0.f;
				this->jumperCounter = 0;
				if (!this->fall)
					this->pos[1] = c[i]->pos[1] - 38;
				this->pos[0] += 1;
			}
			else {
				this->pos[0] = c[i]->pos[0] - 38;
				this->vY = 0.f;
				this->fall = true;
				this->ap->setCurrentAnimation(4);
			}
			break;
		}
	}

	if (!this->fall) {
		if (this->jumperCounter < 2) {
			if (osl_pad.held.cross) {
				this->counter += dt;
				if (this->counter < 0.03)
					this->vY = -10.f;
			}
			else if (osl_pad.pressed.cross)
				this->vY = -1.f;

			if (osl_pad.released.cross) {
				this->jumperCounter++;
				this->counter = 0;
			}
		}

		if (!collision) {
			this->ap->setCurrentAnimation(3);
			if (this->vY < 0 && this->ap->getCurrentFrameIndex() == 3)
				this->ap->pause(true);
			if (this->vY >= 0)
				this->ap->pause(false);
			if (this->ap->getCurrentFrameIndex() == 6)
				this->ap->pause(true);
		}
		else
			this->ap->pause(false);

	}
	else {
		if (!collision) {
			if (this->ap->getCurrentFrameIndex() > 2)
				this->ap->setAnimationFrame(4, 0);
		}
		else this->ap->setAnimationFrame(4, 6);

		if (this->ap->isFinished())
			this->end = true;

	}
	
	if (this->pos[1] > 270)
		this->end = true;


	this->vY = this->vY + this->g * dt;
	this->pos[1] = this->pos[1] + SCALE * (this->vY * dt + 0.5 * this->g * dt * dt);
	return this->end;
}

void Player::draw() {
	oslSetImageTileSize(this->texture, this->currentFrame->x, this->currentFrame->y, this->currentFrame->w, this->currentFrame->h);
	oslDrawImageXY(this->texture, this->pos[0], this->pos[1]);
}

void Player::ia(float dt){
	this->currentFrame = this->ap->update(dt);
	this->size[0] = this->currentFrame->w;
	this->size[1] = this->currentFrame->h;
}