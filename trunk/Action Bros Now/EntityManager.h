//a singleton class that will contain all the entities in the game
//accesses a txt file to load all the enemies for a given lvl
//uses the entity's char id to search spriteContainer and set entity's
//imgptr to proper sprite sheet

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Boss.h"
#include "Enemy.h"
#include "Player.h"
#include "spriteContainer.h"
#include <fstream>
#include <vector>
#include <string>

class EntityManager
{
private:
	std::vector<BaseGameEntity*> entityVector;

public:
	EntityManager();
	//EntityManager(clock_t ct);
	~EntityManager();

	BaseGameEntity* getEntVec(int i)			{ return entityVector[i];}
	std::vector<BaseGameEntity*> getEntVec()	{ return entityVector; }
	
	bool update();
	void moveEnemies(clock_t);
	void updateEnemyState();
	//creates the entity vector for the enemies for this level
	bool loadFromFile(int);		//progress
	void loadFromFile(char* fileName);
	void setImgPtrs();

	int getVecSize()		{ return entityVector.size(); }

	//update the entities
	//void entiyUpdate(Player* player);
};

#endif