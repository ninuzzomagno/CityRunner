#include"Game.h"
PSP_MODULE_INFO("Game", 1, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int main() {
	MyGame().run();
	return 0;
}