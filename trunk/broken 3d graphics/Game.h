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
	void drawLvlVB()		{ graphics->drawLvlVB(); }
	//load level
	bool loadLvl();
	void display_time(clock_t t, int y)		{ graphics->displayTime(t, y); }

	//camera
	void createCam(float n, float f)	{ graphics->createCamera(n, f); }
	void moveCam(D3DXVECTOR3 vec)		{ graphics->moveCamera(vec); }
	void transCam(D3DXVECTOR3 vec)		{ graphics->translateCamera(vec); }
	void pointAndSetCam(D3DXVECTOR3 vec){ graphics->pointAndSetCamera(vec); }

	D3DXVECTOR3 getCamPos()				{ return graphics->getCameraPos(); }
};

#endif