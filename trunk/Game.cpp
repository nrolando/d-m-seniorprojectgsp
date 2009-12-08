#include "Game.h"

Game::Game(clock_t ct)
{
	graphics = new Graphics();
	EntMgr = new EntityManager();
	progress = 0;

	now = ct;
	then = NULL;
}

Game::~Game()
{
	delete graphics;
	delete EntMgr;
}

bool Game::initGame(HWND& hwnd)
{
	return graphics->initD3D(hwnd);
}

void Game::_shutdown()
{
	
}

bool Game::loadLvl()
{
	if(!graphics->loadLvlFromFile(progress))
		return false;

	if(!EntMgr->loadFromFile(progress))
		return false;

	return true;
}

void Game::gameUpdate(clock_t ct)
{
	then = now;
	now = ct;
	beginRender();
	drawLvl();
	display_time((now-then),20);
	endRender();
}
