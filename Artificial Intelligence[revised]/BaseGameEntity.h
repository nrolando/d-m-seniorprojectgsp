#ifndef BASEGAMEENTITY_H
#define	BASEGAMEENTITY_H

#include "Status.h"
#include "Vector.h"
#include <string>

class BaseGameEntity
{
private:
	char entity_ID;
protected:
	Vector2D POS;
	std::string name;
	//void *img;
public:
	BaseGameEntity(char entity_type,Vector2D pos) {entity_ID = entity_type; 
										 name = GetNameOfEntity(entity_type).c_str();
										 POS = pos;}
	
	virtual ~BaseGameEntity(){}

	virtual void UpdateStat(int stat, int val) = 0;
	virtual void UpdateState() = 0;
	//virtual void setImg(/* *DirectXSurface */);
	std::string getName() {return name;}

	int ID() {return entity_ID;}
	Vector2D getPOS() {return POS;}

};

#endif