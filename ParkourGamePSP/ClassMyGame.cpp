#include"Game.h"

MyGame::MyGame() {
	oslInit(OSL_IF_USEOWNCALLBACKS);
	oslInitGfx(OSL_PF_8888, 1);
	oslBenchmarkTestEx(OSL_BENCH_INIT, 0);
	oslSetBkColor(COLOR::trasparent);
	oslShowSplashScreen(1);

	std::fstream FILE("resource/save.SAVE", std::ios::in);
	if (!FILE) {
		FILE.open("resource/save.SAVE", std::ios::out);
		FILE << 0;
		this->mScore = 0;
	}
	else
		FILE >> this->mScore;
	FILE.close();

	Render render;
	float counter = 0.f;
	while (counter<1.f) {
		render.start(COLOR::black);

		oslDrawStringf(200,130,"Load data OK");

		counter += render.end();
	}

}

int MyGame::play(float&score) {
	float dt = 0;
	float vel = 100;
	float dP = 0;
	
	bool pausa = false, end = false;

	ParallaxBackground background("resource/env/cubes/");

	Render render;
	Player* player = nullptr;

	Cubes** c = new Cubes * [5];
	float p[2];
	int s[2];

	oslSrand(time(NULL));

	for (int i = 0; i < 5; i++) {
		s[1] = oslRandf(200, 50);
		p[1] = 272 - s[1];
		if (i > 0) {
			s[0] = oslRandf(200, 100);
			p[0] = p[0] + s[0] + oslRandf(100, 50);
		}
		else {
			p[0] = 0;
			s[0] = 200;
			int pos[2] = { 100,p[1] -38};
			player = new Player(pos,false);
		}
		c[i] = new Cubes(p, s);
	}

	while (true) {
		render.start(COLOR::background);

		dP = vel * dt;

		if (osl_pad.pressed.start && !end)
			pausa = !pausa;
		
		if (!pausa && !end) {

			for (int i = 0; i < 5; i++)
				c[i]->update(dP);

			background.update(dP);
			end = player->update(dt, c);

			vel = !player->isFalling() * 100;

		}

		background.draw();

		for (int i = 0; i < 5; i++)
			c[i]->draw();

		player->draw();

		oslSetTextColor(COLOR::black);
		oslDrawStringf(10, 10, "SCORE : %i", int(score));

		if (pausa) {
			oslDrawFillRect(0, 0, 480, 272, RGBA(0, 0, 0, 125));
			oslSetTextColor(COLOR::white);
			oslPrintf_xy(240, 120, "PAUSA");
		}
		else if (end) {
			oslDrawFillRect(0, 0, 480, 272, RGBA(0, 0, 0, 125));
			oslSetTextColor(COLOR::white);
			oslPrintf_xy(200, 120, "YOU LOSE");
			oslDrawStringf(190, 140, "Score : %i", int(score));


			if (osl_pad.pressed.cross)
				break;
		}

		dt = render.end();
		if(!end && !pausa)
			score += dt;
	}
	return 0;
}

void MyGame::run() {
	int state = 0;
	while (state!=-1) {
		switch (state)
		{
		case 0:
			state = this->menu();
			if (state == 2)
				state = -1;
			break;
		case 1:
			float score = 0;
			state = this->play(score);
			if (this->mScore < score)
				this->mScore = score;
			break;
		}
	}

	std::ofstream FILE("resource/save.SAVE", std::ios::out);
	FILE << this->mScore;

	Render render;
	float counter = 0.f;
	while (counter < 1.f) {
		render.start(COLOR::black);

		oslSetTextColor(COLOR::white);
		oslDrawStringf(200, 130, "Save data OK");

		counter += render.end();
	}

	oslEndGfx();
	oslQuit();

}

int MyGame::menu() {
	ParallaxBackground background("resource/env/cubes/");

	Render render;
	
	float p[2];
	int s[2];

	oslSrand(time(NULL));

	s[1] = oslRandf(200, 50);
	p[1] = 272 - s[1];
	p[0] = 0;
	s[0] = 200;
	int pos[2] = { 180,p[1] - 38 };
	Player player(pos,true);
	Cubes c(p, s);

	float dt = 0;
	int menuSel = 0;

	const char* option[] = { "Play","Exit" };

	while (true) {
		render.start(COLOR::background);

		if (osl_pad.pressed.up && menuSel>0)
			menuSel--;
		else if (osl_pad.pressed.down && menuSel < 1)
			menuSel++;

		if (osl_pad.pressed.cross)
			return menuSel+1;

		player.ia(dt);

		background.draw();
		c.draw();
		player.draw();

		oslSetTextColor(COLOR::black);
		oslDrawFillRect(20, 20, 200, 50, COLOR::white);
		oslDrawStringf(60, 30, "The city runner");

		oslDrawFillRect(250, 20, 450, 50, COLOR::white);
		oslDrawStringf(280, 30, "Your max score : %i", this->mScore);

		oslDrawFillRect(340, 100, 440, 172, COLOR::white);
		for (int i = 0; i < 2; i++) {
			if (menuSel == i) {
				oslSetTextColor(COLOR::black);
				oslDrawStringf(377, 120 + i * 25, option[i]);
			}
			else {
				oslSetTextColor(RGBA(120,120,120,255));
				oslDrawStringf(377, 120 + i * 25, option[i]);
			}
		}

		dt = render.end();
	}
}