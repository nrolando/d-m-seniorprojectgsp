#include "Game.h"

Game::Game()
{
	graphics = new Graphics();
	progress = 0;
}

Game::~Game()
{
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

	return true;
}

