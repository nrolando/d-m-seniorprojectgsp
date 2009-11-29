//a singleton class that will contain all the entities in the game
//accesses a txt file to load all the enemies for a given lvl
//uses the entity's char id to search spriteContainer and set entity's
//imgptr to proper sprite sheet

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "BaseGameEntity.h"
#include "Vector.h"
#include <vector>
#include <fstream>
#include <string>

class EntityManager
{
private:
	//singleton instance
	static EntityManager* instance;
	EntityManager();

	std::vector<BaseGameEntity*> entityVector;

public:
	//singleton access function
	static EntityManager* getInstance();

	void loadFromFile(char* fileName);
	void setImgPtrs();
};

#endif