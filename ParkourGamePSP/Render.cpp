#include"Game.h"

void Render::start(OSL_COLOR c) {
	this->timer.start();
	oslStartDrawing();
	oslClearScreen(c);
	oslReadKeys();
}

float Render::end() {
	oslEndDrawing();
	oslEndFrame();
	oslWaitVSync();
	oslSwapBuffers();

	this->timer.end();
	return this->timer.dt();
}