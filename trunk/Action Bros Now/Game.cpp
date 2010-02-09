#include "Game.h"

//////////TO DO: rEMOVE ALL STATIC VARIAbLES//////////


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
	//MIKE"S CHANGE
	graphics		= new Graphics();
	EntMgr			= new EntityManager();
	player			= new Player("Baek");
	level			= new Level();
	inputMan		= new InputManager2(HI, hWnd);
	input			= ' ';
	screen			= 0;	//start at 1 until we get splash
	currentScreen   = TITLE;
	hitEnemy		= -1;
	lastHitEnemy	= -1;
	lastLvl			= -1;
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
	srand(unsigned(time(0)));

	//initlialize direct sound and load all sounds
	if(!soundManager::getInstance()->initSound(hwnd))
		return false;

	if(!soundManager::getInstance()->loadAllSounds())
		return false;

	if(!graphics->initD3D(hwnd))
		return false;

	if(!graphics->loadSplashTitle())
		return false;

	return true;
}


void Game::_shutdown()
{
	soundManager::getInstance()->shutDown();
}

//MIKE"S CHAnGE: need to figure out how to combine with asset lload function
bool Game::loadLvl()
{
//only call if its a new level, not sublevel
	if(lastLvl != (level->getProg()/3))
	{
	//reload the sprite container for the tiles and enemies for the next level
		if(!graphics->loadSpriteCont(level->getProg()) || !graphics->loadMeters())
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
	hitEnemy = lastHitEnemy = -1;

	//set players data for the next level (parameter should be gotten from Level? constant for now)
	player->setPos(D3DXVECTOR3(-1350.0f, -40.0f, 0.0f));

	//new song per level
	if(soundManager::getInstance()->isBGMplaying())
		soundManager::getInstance()->stopSound();
	soundManager::getInstance()->playSoundLoop(BGMlist[level->getProg()/3]);

	return true;
}

//MIKE"S CHANGE: this loads the initial assets for first lvl after new game is selected
bool Game::loadAssets()
{
	//FILL THE Entity CONTAINER once throughout whole game - players sprites are the first one loaded into the container (see player
	//initialization down below) the sprite container will be loaded in loadlvl()
	if(!graphics->loadEntityCont())
		return false;
	//initialize the players sprite pointer
	player->setSSPtr(spriteContainer::getInstance()->EC_getElem(0));

		return true;
}

bool Game::update(clock_t ct)
{
	char input = '\0';
	static int num = -1;
	bool newGame = true;
	int numdead = 0;

	//get user input
	inputMan->setInput();
	input = inputMan->getInput(screen);

	//temporary save spot for testing
	if(input == ' ')
		this->save();

	switch(screen)
	{
	case 0:		//splash
		soundManager::getInstance()->playSound(BGMlist[5]);
		this->splashScreen();
		if(input == 'p')
			screen++;
		break;
	case 1:		//title/menu
		num = this->titleScreen(input);
		if(num >= 0)
		{
		    graphics->BeginRender();
			graphics->drawLoadScreen();
			graphics->EndRender();

			screen++;
			level->setProg(num);
			//load assets for game (fill entity container, set player SS ptr)
			loadAssets();
			//load level retrieved from titleScreen()
			if(!this->loadLvl())
			{
				MessageBox(NULL, "Unable to load lvl", "ERROR", MB_OK);
				return 1;
			}
		}
		break;
	case 2:
		if(player->getState() != STUN && player->getState() != FALL && player->getState() != RESPAWN)
			player->DoAction(input);
		if(!player->isAlive())
		{
			if(player->getLives() > 0)
				this->respawnPlayer();
			else
			{
				//gameover();
			}
		}

		//check for collision (needs to be moved/adjusted)
		if(player->getHealth() > 0 && player->isAlive())
		{
			hitEnemy = checkAttacks();
			if(hitEnemy >= 0)
				lastHitEnemy = hitEnemy;
		}

		//update player state, enemies state
		player->UpdatePlayerState();
		EntMgr->UpdateEnemyState(player);
		//update entities
		if(!EntMgr->update())
			return false;
		//move player here
		player->move(ct);
		for(int i = 0; i<EntMgr->getVecSize();++i)
			EntMgr->getEntVec(i)->move(ct);
		for(int i=0;i<EntMgr->getVecSize();++i)
		{
			if(!EntMgr->getEntVec(i)->isAlive())
				++numdead;
		}

		//MIKE"S CHANGE: MOVED THE LEVEL END CHECK HERE
		//checks if player beat the level
		if(player->getPos().x > 1350.0f /*&& boss == dead*/)
		{
			if(level->getProg() == 5)
			{
				level->setProg(0);
				if(!graphics->loadSplashTitle())
					return false;
				lastLvl = -1;
				screen = 1;
				currentScreen = TITLE;
				if(soundManager::getInstance()->isBGMplaying())
					soundManager::getInstance()->stopSound();
				return true;
				if(soundManager::getInstance()->isBGMplaying())
					soundManager::getInstance()->stopSound();
				return true;
			}
			else if(numdead == EntMgr->getVecSize())
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
		}

		//EntMgr->moveEnemies(ct);
		//update camera
		if(player->getState() != RESPAWN)
			graphics->updateCamera(player->getDrawInfo());

		//RENDER :D we can put this inside if statements and check to see if anything has changed
		//that way we dont render when not necessary
		graphics->BeginRender();
		graphics->drawLvl(EntMgr->getEntVec(), player->getDrawInfo(), level->getBackTiles(),
			level->getFrontTiles(), (level->getProg()%3));
		if(DEBUGMODE)
			display_time(ct, 50);

		//*******DISPLAY HUD*******
		graphics->DisplayPlayerStat(player->getHealth(),player->getMaxHealth(),player->getSpecial(),player->getMaxSpecial());
		//display Boss health
		if(EntMgr->getStateByKey('B') != SB_IDLE  && EntMgr->getStateByKey('B') >= 0)
			graphics->DisplayBossStat(EntMgr->getHealthByKey('B'), EntMgr->getMaxHealthByKey('B'), EntMgr->getSpecialByKey('B'), EntMgr->getMaxHealthByKey('B'));
		if(EntMgr->getStateByKey('b') != SB_IDLE  && EntMgr->getStateByKey('b') >= 0)
			graphics->DisplayBossStat(EntMgr->getHealthByKey('b'), EntMgr->getMaxHealthByKey('b'), EntMgr->getSpecialByKey('b'), EntMgr->getMaxHealthByKey('b'));
		//display enemy health
		if(!EntMgr->isVectorEmpty() && lastHitEnemy >= 0)
		{
			graphics->DisplayEnemyHealth(EntMgr->getEntVec(lastHitEnemy)->getHealth(),EntMgr->getEntVec(lastHitEnemy)->getMaxHealth());
		}
		graphics->EndRender();
		break;
	case 3:		//gameover/win
		break;
	};

	return true;
}

int Game::checkAttacks()
{
	//index gets either the last enemy you hit, or the last enemy to hit you..return -1 if nothing
	int index = -1;
	eSprInfo pSprInfo = player->getDrawInfo();
	eSprInfo e_SprInfo;
	int distance = 0;	//distance from players threat.top to enemys hit.top
	const int depthRange = 15;	//collision range for distance
	std::vector<BaseGameEntity*> E = EntMgr->getEntVec();

//check player hit enemy collision
	if(player->checkFrames())//makes sure its the first frame only of an attack frame
	{
		for(unsigned int i = 0; i < E.size(); ++i)
		{
			if(E[i]->isAlive() && E[i]->getHealth() > 0)
			{
				//get enemy's info
				e_SprInfo = E[i]->getDrawInfo();
				//check x axis collision boxes
				if(pSprInfo.threatBox.right >= e_SprInfo.hitBox.left && pSprInfo.threatBox.left <= e_SprInfo.hitBox.right)
				{
					//check y axis collision boxes
					if(pSprInfo.threatBox.top >= e_SprInfo.hitBox.bottom && pSprInfo.threatBox.bottom <= e_SprInfo.hitBox.top)
					{
						//check depth collision (z illusion)
						distance = int(pSprInfo.POS.y - (e_SprInfo.POS.y - (e_SprInfo.height/2.0f)));
						if(distance < 0)
							distance *= -1;	//get absolute value
						if(distance < depthRange)
						{
							//play the hit sfx
							//shorten the sfx
							//if(player->getState() == PUNCH || player->getState() == KICK)
								soundManager::getInstance()->playSound("punch_kick_impact");

							//take damage and check if dead
							E[i]->UpdateStat(0, -(player->getDmg()));
							//set the players lastAttackFrame
							player->setLAF(player->getAnimFrame());
							//check if the hit killed the enemy
							if(E[i]->getHealth() < 1)
							{
								E[i]->die();

								//add to player's score
								switch(E[i]->getKey())
								{
								case SOLDIER1:
									player->addScore(50);
									break;
								};
							}
							else
							{
								E[i]->stun();
								index = i;	//last enemy hit
							}
						}
					}
				}
			}
		}
	}

	//check enemies -> player collision
	for(unsigned int i = 0; i < E.size(); ++i)
	{
		if(E[i]->checkFrames())
		{
			e_SprInfo = E[i]->getDrawInfo();
			//check x axis collision boxes
			if(e_SprInfo.threatBox.right >= pSprInfo.hitBox.left && e_SprInfo.threatBox.left <= pSprInfo.hitBox.right)
			{
				//check y axis collision boxes
				if(e_SprInfo.threatBox.top >= pSprInfo.hitBox.bottom && e_SprInfo.threatBox.bottom <= pSprInfo.hitBox.top)
				{
					//check depth collision (z illusion)
					distance = int((e_SprInfo.POS.y - (e_SprInfo.height/2.0f)) - pSprInfo.POS.y);
					if(distance < 0)
						distance *= -1;	//get absolute value
					if(E[i]->getKey() == 'B')
					{
						if(distance < SB_DEPTH_RANGE)
						{
								//play the sword sfx
							//need to shorten the sound
							switch(E[i]->getKey())
							{
							case SOLDIER1:
								if(E[i]->getState() == E_ATTACK1)
									soundManager::getInstance()->playSound("sword_impact");
								break;
							};
							//set last attack frame
							E[i]->setLAF(E[i]->getAnimFrame());

							//TAKE HEALTH FROM PLAYER AND STUN OR DIE
							player->UpdateStat(0, -(E[i]->getPower()));
							player->UpdateStat(1, E[i]->getPower());
							if(player->getHealth() < 1)
								player->die();
							else
								player->stun();

							index = i;		//last enemy that hit you
						}
					}
					else
					{
						if(distance < depthRange)
						{
							//play the sword sfx
							//need to shorten the sound
							switch(E[i]->getKey())
							{
							case SOLDIER1:
								if(E[i]->getState() == E_ATTACK1)
									soundManager::getInstance()->playSound("sword_impact");
								break;
							};
							//set last attack frame
							E[i]->setLAF(E[i]->getAnimFrame());

					//TAKE HEALTH FROM PLAYER AND STUN OR DIE
							player->UpdateStat(0, -(E[i]->getPower()));
							player->UpdateStat(1, E[i]->getPower());
							if(player->getHealth() < 1)
								player->die();
							else
								player->stun();

							index = i;		//last enemy that hit you
						}
					} //else do enemy depth collision
				}
			}
			else
			{E[i]->missedAtk();}
		}

	}
	return index;
}

void Game::respawnPlayer()
{
	D3DXVECTOR3 pPos = player->getPos();
	player->decLives();
	player->setState(RESPAWN);
	for(int i = 0; i < EntMgr->getVecSize(); ++i)
	{
		if(EntMgr->getHealth(i) > 0)
			EntMgr->stunEnt(i, 1000);
	}
	player->setHealth(MAXHEALTH);
	player->setAlive(true);
	player->setVel(D3DXVECTOR3(0.0f, -8.0f, 0.0f));
	pPos.y += 480;
	player->setPos(D3DXVECTOR3(pPos));
	player->resetTimes();
}

int Game::titleScreen(char input)
{

	static int selection = 0;
	switch(input)
	{
	case 'r':
		if(currentScreen == TITLE)
		{
			if(selection < 2)
				selection++;
		}
		break;
	case 'l':
		if(currentScreen == TITLE)
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
		if(currentScreen == TITLE)
		{
			if(selection == 0)		//start new game
				return 0;
			else if(selection == 1)	//go to load screen
			{
				currentScreen = LOAD;
				selection = 0;
			}
			else if(selection == 2)	//go to options screen
			{
				currentScreen = OPTIONS;
				selection = 0;
			}
		}
		else if(currentScreen == LOAD)
		{
			if(this->load())
				return level->getProg();
			else
				MessageBox(NULL, "No load data is available!", "ERROR", MB_OK);
		}
		break;
	case 'k':		//the back button
		if(currentScreen == OPTIONS || currentScreen == LOAD)
		{
			currentScreen = TITLE;
			selection = 0;
		}
		break;
	};
//draw screen
	graphics->BeginRender();
	switch(currentScreen)
	{
	case TITLE:
		graphics->drawTitle(selection);
		if(soundManager::getInstance()->isBGMplaying())
		{
			soundManager::getInstance()->stopSound(BGMlist[5]);
			soundManager::getInstance()->stopSound(BGMlist[6]);
		}
		soundManager::getInstance()->playSoundLoop(BGMlist[3]);
		soundManager::getInstance()->playSoundLoop(BGMlist[4]);
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
	int frameTime;

	if(splashRow == maxRow-1)
		frameTime = 375;
	else
		frameTime = ANIMATIONGAP;

	if(now - aniFStart >= frameTime)
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
	{
		splashRow = 0;
		splashCol = 0;
		screen++;
	}
	else
	{
		//splashCol = 1; splashRow = 7;	//testing purposes
		if(splashCol == 1 && splashRow == 7)
		{
			soundManager::getInstance()->playSound(BGMlist[6]);
			graphics->BeginSplashRender();
		}
		else
			graphics->BeginRender();
		graphics->drawSplash(splashRow, splashCol, width, height);
		graphics->EndRender();
	}
}

bool Game::loadScreen()
{
	clock_t now = clock();
	static int loadCol = 0;
	static int loadRow = 0;
	static int aniFStart = clock();

	const int maxCol = 4;
	const int maxRow = 4;
	const int height = 512;
	const int width = 512;

	if(now - aniFStart >= ANIMATIONGAP)
	{
		if(loadCol < maxCol-1)
			loadCol++;
		else
		{
			loadCol = 0;
			loadRow++;
		}

		aniFStart = now;
	}
	if(loadRow == maxRow)
	{
		loadRow = 0;
		loadCol = 0;
		screen++;
	}
	else
	{
		//loadCol = 1; loadRow = 7;	//testing purposes
		if(loadCol == 1 && loadRow == 4)
		{
			graphics->BeginSplashRender();
			graphics->drawLoadScreen();
			return true;
		}
		else
		{
			graphics->BeginRender();
			graphics->drawLoadAnimation(loadRow, loadCol, width, height);
			graphics->EndRender();
			return false;
		}
	}
	return false;
}

bool Game::save()
{
	ofstream fout;
	fout.open("saved_game.txt");

	if(!fout.is_open())
		return false;

	fout << level->getProg();
	fout << ' ';
	fout << player->getScore();
	fout << ' ';
	fout << player->getLives();

	fout.close();
	return true;
}

//if false is return then there's no saved data
bool Game::load()
{
	int prog;
	int score;
	int lives;
	ifstream fin;

	//open file
	fin.open("saved_game.txt");

	if(!fin.is_open())
		return false;
	//read from file
	fin >> prog;
	fin >> score;
	fin >> lives;
	//set data
	level->setProg(prog);
	player->setScore(score);
	player->setLives(lives);
	//close file
	fin.close();
	return true;
}