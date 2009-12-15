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
//variables we talked about tues 12/1/09 to figure out m_src

//----------------------------------------------------------
protected:
	D3DXVECTOR3 POS;
	std::string name;		//set by getnameofentity(ID);
	int state;				//entities current state

public:
	BaseGameEntity(std::string n) {name = n;}
	BaseGameEntity(std::string n, D3DXVECTOR3 pos)
	{
		name = n;
		POS = pos;
	}
	
	virtual ~BaseGameEntity(){}

	virtual void UpdateStat(int stat, int val) = 0;
	virtual void UpdateState(clock_t) = 0;	

};

#endif