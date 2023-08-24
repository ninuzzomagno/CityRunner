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
	this->flipIA = false;
	if (menu) {
		ap->setCurrentAnimation(1);
		this->pos[0] = -5;
		this->xIA = 180;
		this->jump= nullptr;
	}
	else {
		this->jump = new OSL_SOUND * [2];
		this->jump[0] = oslLoadSoundFileMP3("resource/sounds/jump1.mp3", OSL_FMT_STREAM);
		this->jump[1] = oslLoadSoundFileMP3("resource/sounds/jump2.mp3", OSL_FMT_STREAM);
	}
	this->texture = oslLoadImageFilePNG(const_cast<char*>("resource/player/texture.png"), OSL_IN_RAM, OSL_PF_8888);

	this->playedSound = false;
	this->playedSoundIndex = 0;
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
				this->ap->setAnimationFrame(5, 0);
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
				if(!this->playedSound){
					this->playedSound = true;
					oslPlaySound(this->jump[this->jumperCounter], 2);
					this->playedSoundIndex++;
					if (this->playedSoundIndex == 2)
						this->playedSoundIndex = 0;
				}
				this->counter += dt;
				if (this->counter < 0.2f)
					this->vY = -7.f;
			}

			if (osl_pad.released.cross) {
				this->jumperCounter++;
				this->counter = 0;
				this->playedSound = false;
			}
		}

		if (!collision) {
			if (this->jumperCounter <2 ) {
				this->ap->setCurrentAnimation(3);
				if (this->vY < 0 && this->ap->getCurrentFrameIndex() == 3)
					this->ap->pause(true);
				if (this->vY >= 0)
					this->ap->pause(false);
				if (this->ap->getCurrentFrameIndex() == 6)
					this->ap->pause(true);
			}
			else {
				this->ap->setCurrentAnimation(5);
				if (this->ap->getCurrentFrameIndex() == 6)
					this->ap->pause(true);
			}
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
	if (this->pos[1] < -38)
		oslDrawGradientRect(this->pos[0], 0, this->currentFrame->w + this->pos[0], 10, COLOR::black, COLOR::black, COLOR::trasparent, COLOR::trasparent);
}

void Player::updateIA(float dt){
	this->currentFrame = this->ap->update(dt);
	this->size[0] = this->currentFrame->w;
	this->size[1] = this->currentFrame->h;

	if (abs(this->pos[0] - this->xIA) < 10) {
		if (oslRandf(0.f, 100.f) < 0.5f) {
			this->ap->setCurrentAnimation(0);
			this->vY = 0;
		}
		else {
			this->xIA = oslRandf(0, 180);
			this->ap->setCurrentAnimation(1);
		}
	}
	else if (this->pos[0] > this->xIA) {
		this->flipIA = true;
		vY = -20.f;
	}
	else if(this->pos[0] < this->xIA){
		this->flipIA = false;
		vY = 20.f;
	}

	this->pos[0] += this->vY * dt;
}

void Player::drawIA(float vel) {
	oslSetImageTileSize(this->texture, this->currentFrame->x, this->currentFrame->y, this->currentFrame->w, this->currentFrame->h);
	if (this->flipIA)
		oslMirrorImageH(this->texture);
	oslDrawImageXY(this->texture, this->pos[0] - this->currentFrame->w / 2 + vel, this->pos[1]);
	if (this->flipIA)
		oslMirrorImageH(this->texture);
}