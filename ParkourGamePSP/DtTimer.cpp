#include"DtTimer.h"

DtTimer::DtTimer() {
	this->fps = 0;
	this->delta = 0;
}

void DtTimer::start() {
	oslBenchmarkTestEx(OSL_BENCH_START,0);
}

void DtTimer::end() {
	oslBenchmarkTestEx(OSL_BENCH_END,0);
	this->delta = oslBenchmarkTestEx(OSL_BENCH_GET, 0)/1000000.f;
}

void DtTimer::FPS(int x, int y) {
	if (this->delta!=0) {
		this->fps = 1 / this->delta;
	}
	else
		this->fps = 60;

	if (this->fps > 60)
		this->fps = 60;

	else if (this->fps < 0)
		this->fps = 0;
	oslDrawStringf(x, y, "%i", this->fps);
}