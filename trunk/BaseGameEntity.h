#ifndef BASEGAMEENTITY_H
#define	BASEGAMEENTITY_H

#include "Status.h"
#include "common.h"
#include <string>

#define maxcharsize		50

class BaseGameEntity
{
private:
	int entity_ID;
	char key;
protected:
	Vector2D POS;
	std::string name;		//set by getnameofentity(ID);
	char filename[maxcharsize];
	//void *img;
public:
	BaseGameEntity(int ID) {entity_ID = ID; 
							name = GetNameOfEntity(ID);}
	BaseGameEntity(int ID, char _key, char* fn)
	{
		entity_ID = ID;
		name = GetNameOfEntity(ID);
		key = _key;
		strcpy(filename, fn);
	}
	
	virtual ~BaseGameEntity(){}

	virtual void UpdateStat(int stat, int val) = 0;
	virtual void UpdateState() = 0;
	//virtual void setImg(/* *DirectXSurface */);
	std::string getName() {return name;}

	int ID() {return entity_ID;}
	Vector2D getPOS() {return POS;}

};

#endif