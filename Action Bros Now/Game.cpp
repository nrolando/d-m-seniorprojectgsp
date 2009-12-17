#include "Game.h"

Game::Game()
{
//default constructor shouldn't be called

	graphics = new Graphics();
	EntMgr = new EntityManager();
	player = new Player("Hero!");
	//inputMan = new InputManager2(hInstance,wndHandle);
	progress = 0;
}

//this
Game::Game(HINSTANCE HI, HWND hWnd)
{
	graphics = new Graphics();
	EntMgr = new EntityManager();
	player = new Player("Hero!");
	inputMan = new InputManager2(HI, hWnd);
	progress = 0;
}

Game::~Game()
{
	delete graphics;
	delete EntMgr;
}

bool Game::initGame(HWND& hwnd)
{

	if(!graphics->initD3D(hwnd))
		return false;

	if(!graphics->loadPlayerSS())
		return false;

//initialize the players sprite information
	player->initSprInfo(128, 128, D3DXVECTOR3(50.0f,300.0f,0.0f));

	return true;
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

bool Game::update(clock_t ct)
{
	char input = ' ';

	//get user input
	inputMan->setInput();
	input = inputMan->getInput();

	//THIS IS TEMPORARY INPUT HANDLING
	if(input == 'u')
		moveCamera(D3DXVECTOR3(0.0f, 3.0f, 0.0f));
	if(input == 'd')
		moveCamera(D3DXVECTOR3(0.0f, -3.0f, 0.0f));
	if(input == 'l')
		moveCamera(D3DXVECTOR3(-3.0f, 0.0f, 0.0f));
	if(input == 'r')
		moveCamera(D3DXVECTOR3(3.0f, 0.0f, 0.0f));

//include collision.cpp file for this
	if(player->actionPossible(input))
	{
		//need to make these functions
		player->DoAction(input);
		//write this
		//handleInteractions();
	}


	player->UpdateState(ct);
	EntMgr->update(ct);

//RENDER :D we can put this inside if statements and check to see if anything has changed
//that way we dont render when not necessary
	graphics->BeginRender();
	graphics->drawLvl(EntMgr->getEntVec(), player->getDrawInfo());
	display_time(ct, 20);
	graphics->EndRender();

	return true;
}
/*
void Game::handleInteractions()
{
	//write size()
	for(int i = 0; i <= EntMgr->Size(); i++)
	{
		//bool returning collision function
		if()
		{
			switch(player->getState())
			{
			case ATTACK:
				EntMgr->getEnt(i)->takeDMG(player->getDMG());
				EntMgr->getEnt(i)->Setstate(STUN);
				break;
			case STUN:IDLE:WALK:
				if(EntMgr->getEnt(i)->getState() != STUN)
				{
					player->takeDMG(EntMgr->getEnt(i)->getDMG());
					player->setState(STUN);
				}
				break;
			}
		}
	}
}
*/

void Game::drawLvl()
{
	graphics->drawLvl(EntMgr->getEntVec(), player->getDrawInfo());
}