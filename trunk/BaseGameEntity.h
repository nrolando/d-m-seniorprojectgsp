#ifndef BASEGAMEENTITY_H
#define	BASEGAMEENTITY_H

#include "Status.h"
#include "common.h"
#include <string>
#include <ctime>

#define maxcharsize		50

class BaseGameEntity
{
private:
	int entity_ID;
	char key;
//variables we talked about tues 12/1/09 to figure out m_src
	int anim;
//----------------------------------------------------------
protected:
	D3DXVECTOR3 POS;
	std::string name;		//set by getnameofentity(ID);

public:
	BaseGameEntity(int ID) {entity_ID = ID; 
							name = GetNameOfEntity(ID);}
	BaseGameEntity(int ID, char _key, D3DXVECTOR3 pos)
	{
		entity_ID = ID;
		name = GetNameOfEntity(ID);
		key = _key;
		POS = pos;
	}
	
	virtual ~BaseGameEntity(){}

	virtual void UpdateStat(int stat, int val) = 0;
	virtual void UpdateState() = 0;
	//virtual void setImg(/* *DirectXSurface */);

	//get methods
	std::string getName()	{ return name; }
	//set methods
	int ID() {return entity_ID;}
	


};

#endif