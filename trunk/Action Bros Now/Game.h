#ifndef GAME_H
#define GAME_H

#include "Graphics.h"
#include "EntityManager.h"
#include "inputManager.h"
#include "inputManager2.h"
#include "Level.h"
#include "soundManager.h"
#include <windows.h>

//12-2
//made the graphics stuff private
//added the update function
//the graphics stuff is now handled soely by the engine

class Game
{
private:
	//index to keep track of screen advancement
	int screen;
	char input;
	
	//input Engine
	InputManager2* inputMan;

	//Entities
	Player* player;
	EntityManager *EntMgr;

	Level *level;

	//used to display enemy health bar
	int hitEnemy;
	int lastHitEnemy;

	//Graphics Engine
	Graphics *graphics;

	//graphics func  calls
	void beginRender()		{ graphics->BeginRender();}
	void endRender()		{ graphics->EndRender(); }
	void drawLvl();

	
	
	//load assets
	bool loadAssets();

	//figures figures out what to do about collisions
	//void handleInteractions();
	//bool actionPossible(char input)   {return player->actionPossible(input);}

	void display_time(clock_t t, int y)		{ graphics->displayTime(t, y); }

public:
	Game();
	Game(HINSTANCE, HWND);
	~Game();

	bool initGame(HWND);
	void _shutdown();

//the game update. its passed the elapsed time since the last time it was called from the main game loop
	bool update(clock_t);

//returns progress for game start, if -1, player hasn't chosen yet
	int titleScreen(char);
	void splashScreen();

	//load level
	bool loadLvl();
	

	//player to enemy collision detection
	bool actionPossible(char);
	//checks threat boxes against hit boxes. returns index of last enemy to get hit
	int checkAttacks();

	//camera
	void moveCamera(D3DXVECTOR3 vec)	{ graphics->moveCamera(vec); }

	D3DXVECTOR3 getCamPos()				{ return graphics->getCameraPos(); }
};

#endif