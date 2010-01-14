#include "Game.h"

Game::Game()
{
//default constructor shouldn't be called

	graphics = new Graphics();
	EntMgr = new EntityManager();
	player = new Player("Baek");
	input = ' ';
	//inputMan = new InputManager2(hInstance,wndHandle);
}

//this
Game::Game(HINSTANCE HI, HWND hWnd)
{
	graphics = new Graphics();
	EntMgr = new EntityManager();
	player = new Player("Baek");
	level = new Level();
	input = ' ';
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
		{
			MessageBox(NULL, "Unable to load sprite container", "ERROR", MB_OK);
			return false;
		}
		lastLvl = (level->getProg()/3);
	}

//load the tiles for the next level
	if(!level->loadTiles())
	{
		MessageBox(NULL, "Unable to load tiles", "ERROR", MB_OK);
		return false;
	}
//load the enemies
	if(!EntMgr->loadFromFile(level->getProg()))
	{
		MessageBox(NULL, "Unable to load enemies", "ERROR", MB_OK);
		return false;
	}

	//set players data for the next level (parameter should be gotten from Level? constant for now)
	player->setPos(D3DXVECTOR3(-1350.0f, 0.0f, 0.0f));

	return true;
}

bool Game::update(clock_t ct)
{
	char input = ' ';
	int hitEnemy;
	static bool flag = true;
	bool flag1;

	//get user input
	inputMan->setInput();
	input = inputMan->getInput();

	//checks movement collision
	if(flag1 = actionPossible(input))
	{
		player->DoAction(input);
		flag = true;
		//write this
		//handleInteractions();
	}
	if(flag && !flag1)
	{
		player->setState(IDLE);
		player->setVel(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		player->setAnim(0);
		flag = false;
	}

	//check for collision (needs to be moved/adjusted)
	hitEnemy = checkAttacks();
	
	if(hitEnemy >= 0)
	{
		//drawHealthBars(EntMgr->getEnt(hitEnemy), player);
	}

	//update player state, enemies state
	player->UpdatePlayerState();
	EntMgr->updateEnemyState();
	//move entities
	player->move(ct);

	//checks if player beat the level
	if(player->getPos().x > 1350.0f /*&& boss == dead*/)
	{
		/*automatically move player off level and initiate transmission to next level
		if the player is moving into a whole new level, maybe a score calculation will take place
		before transmission*/
		//increment progress and loadLvl
		level->incrementProg();
		if(!this->loadLvl())
		{
			MessageBox(NULL, "Unable to load level", "ERROR", MB_OK);
			return false;
		}
	}

	EntMgr->moveEnemies(ct);
	//update camera
	graphics->updateCamera(player->getDrawInfo());

//RENDER :D we can put this inside if statements and check to see if anything has changed
//that way we dont render when not necessary
	graphics->BeginRender();
	graphics->drawLvl(EntMgr->getEntVec(), player->getDrawInfo(), level->getBackTiles(),
		level->getFrontTiles(), (level->getProg()%3));
	if(DEBUGMODE)
		display_time(ct, 50);

	graphics->DisplayPlayerStat(player->getHealth(),player->getMaxHealth(),player->getSpecial(),player->getMaxSpecial());
	graphics->DisplayBossStat(player->getHealth(),player->getMaxHealth(),player->getSpecial(),player->getMaxSpecial());
	graphics->DisplayEnemyHealth(EntMgr->getEntVec(0)->getHealth(),EntMgr->getEntVec(0)->getMaxHealth());
	graphics->EndRender();
	

	return true;
}

bool Game::actionPossible(char input)
{
	eSprInfo psi = player->getDrawInfo();
	//Code to check if player new position 
	//is greater than the player's walking area
	//or if the player has collided with something
	if((input == 'u' || input == 'w'  || input == 'x') && psi.POS.y >= YLIMIT_TOP)
		return false;
	if((input == 'd' || input == 'y' || input == 'z') && psi.POS.y <= YLIMIT_BOTTOM)
		return false;
	if((input == 'l' || input == 'z' || input == 'x') && psi.POS.x <= -1500.0f)
		return false;
	return true;
}

int Game::checkAttacks()
{
	int index = -1;
	eSprInfo pSprInfo = player->getDrawInfo();
	eSprInfo e_SprInfo;
	int distance = 0;	//distance from players threat.top to enemys hit.top
	const int depthRange = 10;	//collision range for distance
	std::vector<BaseGameEntity*> E = EntMgr->getEntVec();

	if(player->checkFrames())
	{
		for(unsigned int i = 0; i < E.size(); ++i)
		{
			e_SprInfo = E[i]->getDrawInfo();
			if(pSprInfo.threatBox.right >= e_SprInfo.hitBox.left && pSprInfo.threatBox.left <= e_SprInfo.hitBox.right)
			{
				distance = pSprInfo.threatBox.top - e_SprInfo.hitBox.top;
				if(distance < 0)
					distance *= -1;	//get absolute value
				if(distance < depthRange)
				{
				//we have a collision: //no stun animation yet, so enemy just dies instead. can all be done in
				//one enemy function - takeDmg();
					//enemyTakeDme(player->GetDmg());
					E[i]->setState(CS_DIE);
					E[i]->resetTimes();
					E[i]->setAnim(0);
					player->setLAF(player->getAnimFrame());
					index = i;
				}
			}
		}
	}
	return index;
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