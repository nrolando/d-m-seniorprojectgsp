#ifndef PLAYER_H
#define PLAYER_H
#include "BaseGameEntity.h"

class Player : protected BaseGameEntity
{
private:
	//character attributes
	int health,special,
		pPower,kPower,sPower;
	//character's speed 
	Vector2D velocity;
	bool alive;
public:
	bool actionPossible(char m);
	bool isAlive();
	
	int getHealth() {health;}
	int getSpecialMeter() {special;}
	int getkPower() {kPower;}
	int getsPower() {sPower;}
	int getpPower() {pPower;}

	virtual void UpdateStat(int stat,int val);
	virtual void UpdateState();
	//virtual void setImg(/* *DirectXSurface */);
	//virtual char getName();
};
#endif