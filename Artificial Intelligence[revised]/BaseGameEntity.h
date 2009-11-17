#ifndef BASEGAMEENTITY_H
#define	BASEGAMEENTITY_H

#include "Status.h"
#include "Vector.h"
#include <string>

class BaseGameEntity
{
private:
	int entity_ID;
protected:
	Vector2D POS;
	std::string name;
	//void *img;
public:
	BaseGameEntity(int ID) {entity_ID = ID; 
							name = GetNameOfEntity(ID).c_str();}
	
	virtual ~BaseGameEntity(){}

	virtual void UpdateStat(int stat, int val) = 0;
	virtual void UpdateState() = 0;
	//virtual void setImg(/* *DirectXSurface */);
	std::string getName() {return name;}

	int ID() {return entity_ID;}
	Vector2D getPOS() {return POS;}

};

#endif