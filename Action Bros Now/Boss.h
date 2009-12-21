#ifndef BOSS_H
#define BOSS_H

#include "Status.h"
#include "State.h"
#include "Enemy.h"
#include <string>

//changed from protected to public cuz of error c2243
class Boss : public Enemy
{
private:
	//status_type status;
	int health,special,sPower;
public:

	Boss(int ID);
	Boss(int ID, char KEY, D3DXVECTOR3 pos, spriteSheet *ptr, int w, int h);
	~Boss() {}

	int getSpecialMeter() {return special;}
	int getsPower() {return sPower;}
	
	bool isAlive();

	virtual void UpdateStat(int stat, int val);
	virtual void UpdateState(clock_t);
	
	//virtual void UpdateState();
	//virtual void setImg(/* *DirectXSurface */);
	//virtual char getName();
};

#endif