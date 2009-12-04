#include "Game.h"

Game::Game()
{
	graphics = new Graphics();
	EntMgr = new EntityManager();
	progress = 0;
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

bool Game::update(clock_t l_time)
{
	if(!EntMgr->update(l_time))
		return false;

	return true;
}

bool Game::loadLvl()
{
	if(!graphics->loadLvlFromFile(progress))
		return false;

	if(!EntMgr->loadFromFile(progress))
		return false;

	return true;
}

void Game::drawLvl()
{
	//creating and passing an instance of EntitiyManager to graphics->drawLvl()
	graphics->drawLvl(EntMgr->getEntVec());
}