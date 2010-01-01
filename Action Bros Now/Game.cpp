#include "Game.h"

Game::Game()
{
//default constructor shouldn't be called

	graphics = new Graphics();
	EntMgr = new EntityManager();
	player = new Player("Hero!");
	//inputMan = new InputManager2(hInstance,wndHandle);
}

//this
Game::Game(HINSTANCE HI, HWND hWnd)
{
	graphics = new Graphics();
	EntMgr = new EntityManager();
	player = new Player("Hero!");
	level = new Level();
	inputMan = new InputManager2(HI, hWnd);
}

Game::~Game()
{
	delete graphics;
	delete EntMgr;
	delete level;
	delete player;
}

bool Game::initGame(HWND hwnd)
{

	if(!graphics->initD3D(hwnd))
		return false;

	//set the player's starting level
	level->setProg(0);

//FILL THE Entity CONTAINER - players sprites are the first one loaded into the container (see player
//initialization down below) the sprite container will be loaded in loadlvl()
	if(!graphics->loadEntityCont())
		return false;
//called first here, then called by load level every new level
	if(!graphics->loadSpriteCont(level->getProg()))
		return false;

//initialize the players sprite pointer
	player->setSSPtr(spriteContainer::getInstance()->EC_getElem(0));

	return true;
}

void Game::_shutdown()
{
	
}

bool Game::loadLvl()
{
	static int lastLvl = 0;

//only call if its a new level, not sublevel. and dont call it first time around :)
	if(lastLvl != (level->getProg()/3))
	{
	//reload the sprite container for the tiles and enemies for the next level
		if(!graphics->loadSpriteCont(level->getProg()))
			return false;
		lastLvl = (level->getProg()/3);
	}

//load the tiles for the next level
	if(!level->loadTiles())
		return false;
//load the enemies
	if(!EntMgr->loadFromFile(level->getProg()))
		return false;

	//set players data for the next level (parameter should be gotten from Level? constant for now)
	player->setPos(D3DXVECTOR3(-1200.0f, 0.0f, 0.0f));

	return true;
}

bool Game::update(clock_t ct)
{
	char input = ' ';

	//get user input
	inputMan->setInput();
	input = inputMan->getInput();

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
	graphics->updateCamera(player->getDrawInfo());

//RENDER :D we can put this inside if statements and check to see if anything has changed
//that way we dont render when not necessary
	graphics->BeginRender();
	graphics->drawLvl(EntMgr->getEntVec(), player->getDrawInfo(), level->getTiles(), (level->getProg()%3));
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