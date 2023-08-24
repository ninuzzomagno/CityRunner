#include"Game.h"

MyGame::MyGame() {
	oslInit(OSL_IF_USEOWNCALLBACKS);
	oslInitGfx(OSL_PF_8888, 1);
	oslInitAudioME(OSL_FMT_MP3);
	oslBenchmarkTestEx(OSL_BENCH_INIT, 0);
	oslSetBkColor(COLOR::trasparent);
	oslShowSplashScreen(1);

	std::fstream FILE("resource/save.SAVE", std::ios::in);

	this->music = oslLoadSoundFileMP3("resource/sounds/song.mp3", OSL_FMT_STREAM);

	oslSetSoundLoop(this->music, true);

	Render render;
	float counter = 0.f;

	if (!FILE) {
		FILE.close();
		FILE.open("resoruce/save.SAVE", std::ios::out);
		while (counter < 1.f) {
			render.start(COLOR::black);
			oslDrawStringf(240 - oslGetStringWidth(const_cast<char*>("Save file created")) / 2, 135, "Save file created");
			counter += render.end();
		}
		this->mScore = 0;
	}
	else {
		FILE>>this->mScore;
		while (counter < 1.f) {
			render.start(COLOR::black);
			oslDrawStringf(240 - oslGetStringWidth(const_cast<char*>("Load data  OK")) / 2, 135, "Load data OK");
			counter += render.end();
		}
	}
	FILE.close();

}


int MyGame::play(float&score) {
	float dt = 0;
	float vel = 100, velMax = 100;
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

	int sizeStringPixel = oslGetStringWidth(const_cast<char*>("SCORE : 99999"));
	bool updateVel = false;

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

			vel = !player->isFalling() * velMax;

			if (!updateVel && int(score) % 50 == 0 && int(score) > 0 && velMax < 200) {
				velMax += 10.f;
				updateVel = true;
			}
			else if (updateVel && int(score) % 51 == 0)
				updateVel = false;
		}

		background.draw();

		for (int i = 0; i < 5; i++)
			c[i]->draw(0);

		player->draw();

		oslDrawFillRect(480 - sizeStringPixel - 5, 5, 475, 22, COLOR::white);
		oslSetTextColor(COLOR::black);
		oslDrawStringf(480 - sizeStringPixel, 10, "SCORE : %i", int(score));

		if (pausa) {
			oslDrawFillRect(0, 0, 480, 272, RGBA(0, 0, 0, 125));
			oslSetTextColor(COLOR::white);
			oslPrintf_xy(240 - oslGetStringWidth(const_cast<char*>("PAUSA")) / 2, 120, "PAUSA");
			oslPrintf_xy(240 - oslGetStringWidth(const_cast<char*>("Press CIRCLE to return to main menu")) / 2, 220, "Press CIRCLE to return to main menu");
			oslDrawStringf(240 - oslGetStringWidth(const_cast<char*>("Press START to resume")) / 2, 250, "Press START to resume");
			
			if (osl_pad.pressed.circle)
				break;
		}
		else if (end) {
			oslDrawFillRect(0, 0, 480, 272, RGBA(0, 0, 0, 125));
			oslSetTextColor(COLOR::white);
			oslPrintf_xy(240 - oslGetStringWidth(const_cast<char*>("YOU LOSE")) / 2, 120, "YOU LOSE");
			oslDrawStringf(240 - oslGetStringWidth(const_cast<char*>("Score : %i")) / 2, 140, "Score : %i", int(score));
			oslDrawStringf(240 - oslGetStringWidth(const_cast<char*>("Press CROSS to return to main menu")) / 2, 250, "Press CROSS to return to main menu");

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
	oslPlaySound(this->music, 1);
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

	FILE.close();

	Render render;
	float counter = 0.f;

	oslSetTextColor(COLOR::white);

	while (counter < 1.f) {
		render.start(COLOR::black);
		oslDrawStringf(240 - oslGetStringWidth(const_cast<char*>("Save data OK")) / 2, 135, "Save data OK");
		counter += render.end();
	}

	oslDeinitAudio();
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

	const char* option[] = { "Play","Controls","Exit" };

	int alpha = 255;

	std::string scorestring;

	{
		std::stringstream ss;
		ss << "Highest score : " << this->mScore;
		std::getline(ss, scorestring);
	}

	int transition = 0;
	float vel = 0;
	bool ctrl = false;

	while (true) {
		render.start(COLOR::background);
		
		if (alpha <= 0) {
			if (!ctrl) {
				if (osl_pad.pressed.up && menuSel > 0)
					menuSel--;
				else if (osl_pad.pressed.down && menuSel < 2)
					menuSel++;

				if (osl_pad.pressed.cross) {
					if (menuSel == 1) {
						transition = -1;
						ctrl = true;
					}
					else if (menuSel == 0)
						return 1;
					else if (menuSel == 2)
						return 2;
				}
			}
			else if (vel == -480.f) {
				if (osl_pad.pressed.circle)
					transition = 1;
			}

		}
		else
			alpha -= 5;

		if (osl_pad.pressed.square) {
			this->mScore = 0;
			std::stringstream ss;
			ss << "Highest score : " << this->mScore;
			std::getline(ss, scorestring);
		}

		if (transition != 0)
			vel += 300 * dt * transition;

		if (vel > 0) {
			vel = 0;
			transition = 0;
			ctrl = false;
		}
		else if (vel < -480) {
			vel = -480;
			transition = 0;
		}
		player.updateIA(dt);

		background.drawIA(vel);

		c.draw(vel);

		oslSetTextColor(COLOR::black);
		oslDrawFillRect(20 + vel, 20, 200 + vel, 50, COLOR::white);
		oslDrawStringf(60 + vel, 30, "The city runner");

		player.drawIA(vel);

		oslDrawFillRect(250 + vel, 20, 450 + vel, 50, COLOR::white);
		oslDrawStringf(350 + vel - oslGetStringWidth(const_cast<char*>(scorestring.c_str())) / 2, 30, scorestring.c_str(), this->mScore);

		oslDrawFillRect(320 + vel, 100, 460 + vel, 198, COLOR::white);
		for (int i = 0; i < 3; i++) {
			int wM = oslGetStringWidth(const_cast<char*>(option[i])) / 2;
			if (menuSel == i) {
				oslSetTextColor(COLOR::black);
				oslDrawStringf(390 - wM - 10 - oslGetStringWidth("--") + vel, 120 + i * 25, "--");
				oslDrawStringf(390 - wM + vel, 120 + i * 25, option[i]);
				oslDrawStringf(390 + wM + 10 + vel, 120 + i * 25, "--");
			}
			else {
				oslSetTextColor(RGBA(120,120,120,255));
				oslDrawStringf(390 - wM + vel, 120 + i * 25, option[i]);
			}
		}

		{
			oslDrawFillRect(480 + 130 + vel, 53, 480 + 350 + vel, 242, COLOR::white);
			oslSetTextColor(COLOR::black);
			oslDrawStringf(480 + 240 - oslGetStringWidth("CONTROLS") / 2 + vel, 73, "CONTROLS");

			oslDrawStringf(480 + 240 - oslGetStringWidth("CROSS : jump / double jump") / 2 + vel, 123, "CROSS : jump / double jump");
			oslDrawStringf(480 + 240 - oslGetStringWidth("SQUARE : reset highest score") / 2 + vel, 143, "SQUARE : reset highest score");
			oslDrawStringf(480 + 240 - oslGetStringWidth("START : pause / resume") / 2 + vel, 189, "START : pause / resume");

			oslSetTextColor(COLOR::black);
			oslDrawFillRect(485 + vel, 247, 590 + vel, 267, COLOR::white);
			oslDrawStringf(490 + vel, 255, "Version : %.2f", VERSION);
		}

		oslDrawFillRect(0, 0, 480, 272, RGBA(0, 0, 0, alpha));

		dt = render.end();
	}  
}