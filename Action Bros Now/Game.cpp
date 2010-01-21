#include "Game.h"

Game::Game()
{
//default constructor shouldn't be called

	graphics = new Graphics();
	EntMgr = new EntityManager();
	player = new Player("Baek");
	input = ' ';
	screen = 0;
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
	screen = 0;	//start at 1 until we get splash
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

	/*
	//PROBLEMS IN THESE FUCTIONS//
	if(!soundManager::getInstance()->initSound(hwnd))
		return false;
	if(!soundManager::getInstance()->loadAllSounds())
		return false;
	*/
	return true;
}

void Game::_shutdown()
{
	//PROBLEMS
	soundManager::getInstance()->shutDown();
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
	int num;
	int hitEnemy;
	static bool flag = true;
	bool flag1 = true;
	bool newGame = true;

	//something is wrong
	

	//get user input
	inputMan->setInput();
	input = inputMan->getInput(screen);
	
	if(input == 'p')
		playBGM();

	switch(screen)
	{
	case 0:		//splash
		this->splashScreen();
		break;
	case 1:		//title/menu
		num = this->titleScreen(input);
		if(num >= 0)
		{
			screen++;
			level->setProg(num);
			//load that level
			if(!this->loadLvl())
			{
				MessageBox(NULL, "Unable to load lvl", "ERROR", MB_OK);
				return 1;
			}
		}
		break;
	case 2:		//game
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
	EntMgr->UpdateEnemyState(player->getPos());
	//move entities
	player->move(ct);
	for(int i = 0; i<EntMgr->getVecSize();++i)
		EntMgr->getEntVec(i)->move(ct);

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

	//EntMgr->moveEnemies(ct);
	//update camera
	graphics->updateCamera(player->getDrawInfo());

	//RENDER :D we can put this inside if statements and check to see if anything has changed
	//that way we dont render when not necessary
		graphics->BeginRender();
		graphics->drawLvl(EntMgr->getEntVec(), player->getDrawInfo(), level->getBackTiles(),
			level->getFrontTiles(), (level->getProg()%3));
		if(DEBUGMODE)
			display_time(ct, 50);

		//graphics->DisplayPlayerStat(player->getHealth(),player->getMaxHealth(),player->getSpecial(),player->getMaxSpecial());
		//graphics->DisplayBossStat(player->getHealth(),player->getMaxHealth(),player->getSpecial(),player->getMaxSpecial());
		//graphics->DisplayEnemyHealth(EntMgr->getEntVec(0)->getHealth(),EntMgr->getEntVec(0)->getMaxHealth());
		graphics->EndRender();
		break;
	case 3:		//gameover/win
		break;
	};

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
					E[i]->setState(E_DIE);
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

int Game::titleScreen(char input)
{
	static SCREENS current_screen = TITLE;
	//title: 0 - new game; 1 - load game; 2 - options
	static int selection = 0;

	switch(input)
	{
	case 'r':
		if(current_screen == TITLE)
		{
			if(selection < 2)
				selection++;
		}
		break;
	case 'l':
		if(current_screen == TITLE)
		{
			if(selection > 0)
				selection--;
		}
		break;
	case 'd':
		break;
	case 'u':
		break;
	case 'p':	//the select button
		if(current_screen == TITLE)
		{
			if(selection == 0)		//start new game
				return 0;
			else if(selection == 1)	//go to load screen
			{
				current_screen = LOAD;
				selection = 0;
			}
			else if(selection == 2)	//go to options screen
			{
				current_screen = OPTIONS;
				selection = 0;
			}
		}
		if(current_screen == LOAD)
		{
			//return loadGame();
		}
		break;
	case 'k':		//the back button
		if(current_screen == OPTIONS || current_screen == LOAD)
		{
			current_screen = TITLE;
			selection = 0;
		}
		break;
	};

//draw screen
	graphics->BeginRender();
	switch(current_screen)
	{
	case TITLE:
		graphics->drawTitle(selection);
		break;
	case LOAD:
		break;
	case OPTIONS:
		break;
	};
	graphics->EndRender();

	return -1;
}

void Game::splashScreen()
{
	clock_t now = clock();
	static int splashCol = 0;
	static int splashRow = 0;
	static int aniFStart = clock();

	const int maxCol = 5;
	const int maxRow = 10;
	const int height = 400;
	const int width = 400;

	if(now - aniFStart >= ANIMATIONGAP)
	{
		if(splashCol < maxCol-1)
			splashCol++;
		else
		{
			splashCol = 0;
			splashRow++;
		}

		aniFStart = now;
	}
	if(splashRow == maxRow)
		screen++;
	else
	{
		//splashCol = 1; splashRow = 7;	//testing purposes
		if(splashCol == 1 && splashRow == 7)
			graphics->BeginSplashRender();
		else
			graphics->BeginRender();
		graphics->drawSplash(splashRow, splashCol, width, height);
		graphics->EndRender();
	}
}