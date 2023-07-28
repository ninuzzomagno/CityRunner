#pragma once
#include"timeManager.h"
#include"oslib/oslib.h"

class DtTimer {
public:
	DtTimer();
	void start();
	void end();
	inline float dt() { return this->delta; };
	void FPS(int, int);
private:
	float delta;
	int fps;
};