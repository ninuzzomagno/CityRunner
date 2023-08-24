#include"Game.h"

void AnimationPlayer::setAnimationFrame(int anm, int frame) {
	this->anmList[anm]->setFrame(frame);
}

AnimationPlayer::AnimationPlayer(std::string path) {

	std::ifstream file(path,std::ios::in);

	file>>this->maxAnm;
	file >> this->currentAnimation;
	this->defaultAnimation = this->currentAnimation;

	this->anmList = new Animation*[this->maxAnm];

	std::string p;

	for (int i = 0; i < this->maxAnm; i++) {
		file >> p;
		this->anmList[i] = new Animation(p);
	}

	file.close();
}

IRect* AnimationPlayer::update(float dt) {
	if (this->anmList[this->currentAnimation]->isFinished() && !this->anmList[this->currentAnimation]->isLoop()) {
		this->anmList[this->currentAnimation]->reset();
		this->currentAnimation = this->defaultAnimation;
	}
	return this->anmList[this->currentAnimation]->update(dt);
}

AnimationPlayer::~AnimationPlayer() {
	for (int i = 0; i < this->maxAnm; i++)
		delete this->anmList[i];
	delete[]this->anmList;
	delete this->anmList;
}

Animation::Animation(std::string path) {
	std::ifstream FILE(path, std::ios::in);

	this->pausa = false;
	this->finish = false;
	this->currentFrame = 0;
	this->counter = 0;

	FILE >> this->maxFrame;
	FILE >> this->iniFrame;
	FILE >> this->loop;
	FILE >> this->duration;

	this->frames = new IRect[this->maxFrame];

	for (int i = 0; i < this->maxFrame; i++) {
		FILE >> this->frames[i].x;
		FILE >> this->frames[i].y;
		FILE >> this->frames[i].w;
		FILE >> this->frames[i].h;
	}

	FILE.close();
}

IRect* Animation::update(float dt) {
	if (this->pausa == false) {
		this->counter += dt;
		if (this->counter >= (this->duration / this->maxFrame)) {
			this->counter = 0;
			this->currentFrame += 1;
			if (this->currentFrame == this->maxFrame) {
				if (this->loop)
					this->currentFrame = this->iniFrame;
				else
					this->currentFrame -= 1;
				this->finish = true;  // da scrivere in modo diverso
			}
		}
	}

	return &this->frames[this->currentFrame];

}

void Animation::reset() {
	this->counter = 0;
	this->currentFrame = 0;
	this->pausa = false;
	this->finish = false;
}

Animation::~Animation() {
	delete this->frames;
}