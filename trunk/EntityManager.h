//a singleton class that will contain all the entities in the game
//accesses a txt file to load all the enemies for a given lvl
//uses the entity's char id to search spriteContainer and set entity's
//imgptr to proper sprite sheet

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Boss.h"
#include "Enemy.h"
//#include "common.h"
#include <fstream>
#include <vector>
#include <string>

class EntityManager
{
private:
	std::vector<BaseGameEntity*> entityVector;

public:
	EntityManager();
	~EntityManager();
	
	bool loadFromFile(int);		//progress
	void setImgPtrs();
};

#endif