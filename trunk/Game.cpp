#include "Game.h"


//need additional parameters for the input device
Game::Game(clock_t ct, HINSTANCE hInstance, HWND wndHandle)
{
	int i = 1;
	inputMan = new InputManager2(hInstance,wndHandle);
	graphics = new Graphics();
	EntMgr = new EntityManager();
	player = new Player(i); // NEED TO add to the constructor
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

void Game::gameUpdate()
{
	//then = now;
	//now = ct;
	char input;
	
	inputMan->setInput();

	input = inputMan->getInput();


	//THIS IS TEMPORARY INPUT HANDLING
	if(input == 'u')
		moveCamera(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	if(input == 'd')
		moveCamera(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
	if(input == 'l')
		moveCamera(D3DXVECTOR3(-100.0f, 0.0f, 0.0f));
	if(input == 'r')
		moveCamera(D3DXVECTOR3(100.0f, 0.0f, 0.0f));

	//include collision.cpp file for this
	//if(actionPossible(input))
	//{
	//	//need to make these functions
	//	Player->doThis(input);
	//	//write this
	//	handleInteractions();
	//}

	//Player->update(clock());

	//EntMgr->update(clock());

	beginRender();
	drawLvl();
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!need to write these functions!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//drawEntity(player->getPos(), player->getSheet(), player->getSRect());
	/*for(int i = 0; i <= EntMgr->size(); i++) need to make size()
	{
		drawEntity(EntMgr->getEnt(i)->getPos(), EntMgr->getEnt(i)->getSheet(),
			EntMgr->getEnt(i).getSRect()); 
	}*/
	display_time((now-then),20);
	endRender();
}


void Game::handleInteractions()
{
	////write size()
	//for(int i = 0; i <= EntMgr->Size(); i++)
	//{
	//	//bool returning collision function
	//	if()
	//	{
	//		switch(player->getState())
	//		{
	//		case ATTACK:
	//			EntMgr->getEnt(i)->takeDMG(player->getDMG());
	//			EntMgr->getEnt(i)->Setstate(STUN);
	//			break;
	//		case STUN:IDLE:WALK:
	//			if(EntMgr->getEnt(i)->getState() != STUN)
	//			{
	//				player->takeDMG(EntMgr->getEnt(i)->getDMG());
	//				player->setState(STUN);
	//			}
	//			break;
	//		}
	//	}
	//}
}