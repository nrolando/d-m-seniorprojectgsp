#ifndef BOSS_H
#define BOSS_H

#include "Status.h"
#include "State.h"
#include "Enemy.h"
#include <string>

class Player;

//changed from protected to public cuz of error c2243
class Boss : public Enemy
{
private:
	State<Boss,Player> *CurrentState;
	int health,special,sPower;
public:

	Boss(int ID);
	Boss(int ID, char KEY, D3DXVECTOR3 pos, spriteSheet *ptr);
	~Boss() {}

	int getSpecialMeter() {return special;}
	int getsPower() {return sPower;}
	
	bool isAlive();

	virtual void UpdateStat(int stat, int val);
	virtual void UpdateState(Player*);

	
	void ChangeState(State<Boss,Player>* pNewState);
	int getStatus() {return state;}
};

#endif