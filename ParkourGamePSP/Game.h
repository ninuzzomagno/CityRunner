#pragma once

#include<oslib/oslib.h>
#include<math.h>
#include<string>
#include<fstream>
#include<time.h>

#include"DtTimer.h"

const float SCALE = 15.f;

enum class DIRECTION { null = -1, left = 0, up = 1, right = 2, bottom = 3 };

class COLOR { 
public:
	const static OSL_COLOR trasparent = RGBA(0, 0, 0, 0);
	const static OSL_COLOR black = RGBA(0, 0, 0, 255);
	const static OSL_COLOR white = RGBA(255, 255, 255, 255);
	const static OSL_COLOR red = RGBA(255, 0, 0, 255);
	const static OSL_COLOR blue = RGBA(0, 0, 255, 255);
	const static OSL_COLOR green = RGBA(0, 255, 0, 255);
	const static OSL_COLOR background = RGBA(30, 148, 110, 255);
};

struct IRect {
	int x, y, w, h;
};

class Animation {
public:
	Animation(std::string);
	~Animation();
	inline int getCurrentFrameIndex() { return this->currentFrame; };
	inline void setFrame(int f) { this->currentFrame = f; };
	inline bool isLoop() { return this->loop; };
	inline bool isFinished() { return this->finish; };
	IRect* update(float);
	inline void pause(bool v) { this->pausa = v; };
	void reset();
private:
	IRect* frames;
	int maxFrame, iniFrame, currentFrame;
	bool pausa, finish, loop;
	float duration, counter;
};

class AnimationPlayer {
public:
	inline bool isFinished() { return this->anmList[this->currentAnimation]->isFinished(); };
	AnimationPlayer(std::string);
	~AnimationPlayer();
	inline int getCurrentAnimation() { return this->currentAnimation; };
	void setAnimationFrame(int, int);
	inline void pause(bool v) { this->anmList[this->currentAnimation]->pause(v); };
	inline int getCurrentFrameIndex() { return this->anmList[this->currentAnimation]->getCurrentFrameIndex(); };
	inline void setCurrentAnimation(int i) { this->currentAnimation = i; }
	IRect* update(float);
private:
	Animation** anmList;
	int currentAnimation;
	int defaultAnimation;
	int maxAnm;
};

class Render {
public:
	void start(OSL_COLOR);
	float end();
private:
	DtTimer timer;
};

class MyGame {
public:
	MyGame();
	int menu();
	void run();
	int play(float&);
private:
	int mScore;
};

class Cubes {
public:
	Cubes();
	Cubes(float*, int*);
	bool collide(Cubes*);
	void update(float);
	void draw();

	float pos[2];
	int size[2];
};

class Player : Cubes {
public:
	Player(int*,bool);
	bool update(float, Cubes**);
	void ia(float);
	void draw();
	inline bool isFalling() { return this->fall; };
private:
	bool menu, fall, end;
	int jumperCounter;
	float vY, g, counter;
	OSL_IMAGE* texture;
	IRect* currentFrame;
	AnimationPlayer* ap;
};

class ParallaxLayer {
public:
	ParallaxLayer(int, int, float);

	void update(float);

	float pos[2];
	float vel;
	int index;
};

class ParallaxBackground {
public:
	ParallaxBackground(std::string path);
	void update(float);
	void draw();
private:

	ParallaxLayer** layers;
	int maxLayers;
	int multi;
	IRect* frames;
	OSL_IMAGE** text;
	int maxFrame;
};



