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
	char fname[maxcharsize];
	//ENTITY VARIABLES THAT WILL BE PASSES TO THE CONSTRUCTOR
	char map_key = ' ';
	Vector2D PoS;
	Sprite *s_ptr;
	int id = 0;
	int w; int h;

	while(i < maxcharsize)	//set all fname = \0
	{
		fname[i] = NULL;
		++i;
	}
	i = 0;

	sprintf_s(fname, (size_t)maxcharsize, "./lvl%isprites/mobs%i-%i.txt", lvl, lvl, sublvl);

	inFile.open(fname);
	if(!inFile.is_open())
		return false;

	while(i < maxcharsize)	//reset fname back to NULL
	{
		fname[i] = NULL;
		++i;
	}
	i = 0;
	//start reading from the mobs.txt files||get key, position, frame dimension
	inFile.get(map_key);
	while(!inFile.eof())
	{
		inFile >> PoS.x;
		inFile >> PoS.y;
		inFile >> w;
		inFile >> h;
		inFile.ignore();

		for(i = 0; i < spriteContainer::getInstance()->size(); i++)
		{
			if(map_key == spriteContainer::getInstance()->getElem(i)->s)
				s_ptr = spriteContainer::getInstance()->getElem(i);
		}
		
		if(map_key == 'b' || map_key == 'B')
			entityVector.push_back(new Boss(id, map_key, PoS, s_ptr, w, h));
		else
			entityVector.push_back(new Enemy(id, map_key, PoS, s_ptr, w, h));

		id++;
		inFile.get(map_key);
	}
	inFile.close();

	return true;
}

bool EntityManager::update(clock_t _time)
{
//this is all for testing atm; donnie, this is where you take over

	for(int i = 0; i < entityVector.size(); i++)
	{
		entityVector[i]->UpdateState(_time);
	}

	return true;

	//iterate through the vector list
	/*std::vector<BaseGameEntity*>::size_type vsz = entityVector.size();
	for(int i = 0; i < vsz; ++i)
	{*/
		/*if(player.hitbox collide with entityVector[i].hitbox)
		{
			switch(player.getstate())
			{
			case attack:
				entityVector[i].takedmg(player.dealdmg());
				entityVector[i].setStun();
				break;
			case stun:
				if(entityVector[i].getstate() == attacking)
					player.takedmg(entityVector[i].dealdmg());
					player.setstun();
				break;
			default:
				if(entityVector[i].getstate() == attacking)
					player.takedmg(entityVector[i].dealdmg());
					player.setstun();
				break;
			}
		}*/
}