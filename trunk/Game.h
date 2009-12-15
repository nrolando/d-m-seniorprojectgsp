#ifndef GAME_H
#define GAME_H

#include "inputManager2.h"
#include "Graphics.h"
#include "Player.h"
#include "EntityManager.h"
#include <windows.h>

//12-2
//made the graphics stuff private 
//added the update function
//the graphics stuff is now handled soely by the engine

class Game
{
private:
	//players recorded progress
	int progress;

	//stuff
	clock_t then;
	clock_t now;

	//input Engine
	InputManager2* inputMan;
	//Graphics Engine
	Graphics* graphics;
	//entities
	EntityManager* EntMgr;
	//player
	Player* player;

	//figures figures out what to do about collisions
	void handleInteractions();

	//graphics func  calls
	void beginRender()		{ graphics->BeginRender();}
	void endRender()		{ graphics->EndRender(); }
	void drawLvl()			{ graphics->drawLvl(EntMgr->getEntVec()); }

	void display_time(clock_t t, int y)		{ graphics->displayTime(t, y); }

public:
	Game(clock_t ct, HINSTANCE hInstance, HWND wndHandle);
	~Game();

	bool initGame(HWND&);
	void _shutdown();

	void gameUpdate();

	void setProg(int p)		{ progress = p; }
	
	//load level
	bool loadLvl();

	//camera
	void moveCamera(D3DXVECTOR3 vec)		{ graphics->moveCamera(vec); }

	D3DXVECTOR3 getCamPos()				{ return graphics->getCameraPos(); }

	void gameUpdate(clock_t ct);
};

#endif