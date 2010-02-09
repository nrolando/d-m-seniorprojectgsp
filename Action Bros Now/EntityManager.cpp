#include "entityManager.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

//pretty rudimentary
//file format is assumed as:
//t 2 34 (type x y)

//creates the entity vector for the enemies for this level
bool EntityManager::loadFromFile(int prog)
{
	std::ifstream inFile;
	//const int maxcharsize = 50;
	unsigned int i = 0; int j = 0;
	int lvl = prog/3;
	int sublvl = prog%3;
	char fname[MAXCHARSIZE];
	//ENTITY VARIABLES THAT WILL BE PASSES TO THE CONSTRUCTOR
	char map_key = ' ';
	D3DXVECTOR3 PoS;
	spriteSheet *ss_ptr;
	int id = 0;

	if(!entityVector.empty())
		entityVector.clear();

	sprintf_s(fname, (size_t)MAXCHARSIZE, "./enemySprites/mobs%i-%i.txt", lvl, sublvl);

	inFile.open(fname);
	if(!inFile.is_open())
		return false;

	//start reading from the mobs.txt files||get key, position, frame dimension
	inFile.get(map_key);
	while(!inFile.eof())
	{
		inFile >> PoS.x;
		inFile >> PoS.y;
		PoS.z = 0.0f;		//z will be modified later
		inFile.ignore();

		for(i = 0; i < spriteContainer::getInstance()->EC_size(); i++)
		{
			if(map_key == spriteContainer::getInstance()->EC_getElem(i)->key)
				ss_ptr = spriteContainer::getInstance()->EC_getElem(i);
		}
		
		if(map_key == 'b' || map_key == 'B')
			entityVector.push_back(new Boss(id, map_key, PoS, ss_ptr));
		else
			entityVector.push_back(new Enemy(id, map_key, PoS, ss_ptr));

		id++;
		inFile.get(map_key);
	}
	inFile.close();

	return true;
}

void EntityManager::UpdateEnemyState(Player *p)
{
	for(unsigned int i = 0; i < entityVector.size(); i++)
	{
		entityVector[i]->UpdateState(p,this->getEntVec());
	}
}

void EntityManager::moveEnemies(clock_t et)
{
	for(unsigned int i = 0; i < entityVector.size(); i++)
	{
		entityVector[i]->move(et);
	}
}

bool EntityManager::update()
{
	if(entityVector.empty())
		return true;
	//iterate through the vector list
	std::vector<BaseGameEntity*>::iterator iter = entityVector.begin();

	/*
	//check if any enemies are dead and erase them
	for(; iter != entityVector.end(); ++iter)
	{
		if(!(*iter)->isAlive())
			iter = entityVector.erase(iter);
	}
	*/
	return true;
}

int EntityManager::getStateByKey(char k)
{
	for(unsigned int i = 0; i < entityVector.size(); ++i)
	{
		if(entityVector[i]->getKey() == k)
		{
			return entityVector[i]->getState();
		}
	}
	return 0;
}

int EntityManager::getHealthByKey(char k)
{
	for(unsigned int i = 0; i < entityVector.size(); ++i)
	{
		if(entityVector[i]->getKey() == k)
		{
			return entityVector[i]->getHealth();
		}
	}
	return -1;
}

int EntityManager::getMaxHealthByKey(char k)
{
	for(unsigned int i = 0; i < entityVector.size(); ++i)
	{
		if(entityVector[i]->getKey() == k)
		{
			return entityVector[i]->getMaxHealth();
		}
	}
	return -1;
}

int EntityManager::getSpecialByKey(char k)
{
	for(unsigned int i = 0; i < entityVector.size(); ++i)
	{
		if(entityVector[i]->getKey() == k)
		{
			return entityVector[i]->getSpecial();
		}
	}
	return -1;
}

int EntityManager::getMaxSpecialByKey(char k)
{
	for(unsigned int i = 0; i < entityVector.size(); ++i)
	{
		if(entityVector[i]->getKey() == k)
		{
			return entityVector[i]->getMaxSpecial();
		}
	}
	return -1;
}