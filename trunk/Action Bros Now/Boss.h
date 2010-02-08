#ifndef BOSS_H
#define BOSS_H

#include "Status.h"
#include "State.h"
#include "Enemy.h"
#include <string>

class Player;
class EntityManager;

//changed from protected to public cuz of error c2243
class Boss : public BaseGameEntity
{
private:
	State<Boss, Player> *CurrentState;
	int health,maxSpecial,special,sPower;
	bool AnimFinished;
public:

	Boss(int ID);
	Boss(int ID, char KEY, D3DXVECTOR3 pos, spriteSheet *ptr);
	~Boss() {}

	int getSpecial() {return special;}
	int getMaxSpecial() {return maxSpecial;}
	int getsPower() {return sPower;}
	int getDistance(Boss*,Player*);
	
	bool isAlive();
	bool isAnimFinished() {return AnimFinished;}

	virtual void UpdateStat(int stat, int val);
	virtual void calcDrawRECT();
	virtual void UpdateState(Player*,std::vector<BaseGameEntity*>);
	virtual void stun();
	virtual void stun(int);
	virtual void die() {};

	
	void ChangeState(State<Boss, Player>* pNewState);
	void movement(char);
};

#endif