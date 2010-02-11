#ifndef GAME_H
#define GAME_H

#include "Graphics.h"
#include "EntityManager.h"
#include "inputManager.h"
#include "inputManager2.h"
#include "BossOwnedStates.h"
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

	//will be in the Menu class later
	SCREENS currentScreen;

	char input;
	
	//level tracker
	Level *level;

	//input Engine
	InputManager2* inputMan;

	//Entities
	Player* player;
	EntityManager *EntMgr;
	//used to display bosses health
	int bAggression, BAggression;

	//used to display enemy health bar
	int hitEnemy;
	int lastHitEnemy;
	//MIKE"S CHANGES
	//used for level transition
	int lastLvl;

	//Graphics Engine
	Graphics *graphics;

	//graphics func  calls
	//void beginRender()		{ graphics->BeginRender();}
	//void endRender()		{ graphics->EndRender(); }
	void drawLvl();

	//load assets
	bool loadAssets();

	//save/load
	bool save();
	bool load();

	void display_time(clock_t t, int y)		{ graphics->displayTime(t, y); }
	void lvlTrans(clock_t);		//move player off level

public:
	Game();
	Game(HINSTANCE, HWND);
	~Game();

	//functions for input when switching windows
	void activateInput()					{inputMan->acquireDevice();}
	void deavtivateInput()					{inputMan->unAcquireDevice();}

	bool initGame(HWND);
	void _shutdown();

//the game update. its passed the elapsed time since the last time it was called from the main game loop
	bool update(clock_t);

//returns progress for game start, if -1, player hasn't chosen yet
	int titleScreen(char);
	void splashScreen();
	bool loadScreen();

	//load level
	bool loadLvl();
	
	//checks threat boxes against hit boxes. returns index of last enemy to get hit
	int checkAttacks();

	void respawnPlayer();
	bool resetGameToTitle();

	//camera
	void moveCamera(D3DXVECTOR3 vec)	{ graphics->moveCamera(vec); }
	D3DXVECTOR3 getCamPos()				{ return graphics->getCameraPos(); }
};

#endif