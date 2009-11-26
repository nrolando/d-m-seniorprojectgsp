#ifndef GAME_H
#define GAME_H

#include "Graphics.h"

class Game
{
private:
	//players recorded progress
	int progress;

	//Graphics Engine
	Graphics *graphics;

public:
	Game();
	~Game();

	bool initGame(HWND&);
	void _shutdown();

	void setProg(int p)		{ progress = p; }

//************GRAPHICS METHODS*******************
	void beginRender()		{ graphics->BeginRender();}
	void endRender()		{ graphics->EndRender(); }
	void drawLvl()			{ graphics->drawLvl(); }
	//load level
	bool loadLvl();
	void display_time(clock_t t, int y)		{ graphics->displayTime(t, y); }

	//camera
	void moveCamera(D3DXVECTOR3 vec)		{ graphics->moveCamera(vec); }

	D3DXVECTOR3 getCamPos()				{ return graphics->getCameraPos(); }
};

#endif