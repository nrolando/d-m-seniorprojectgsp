#ifndef PLAYER_H
#define PLAYER_H
#include "BaseGameEntity.h"

class Player : protected BaseGameEntity
{
private:
	//character attributes
	int health,special,
		pPower,kPower,sPower, lives;
	//character's speed/movement 
	Vector2D velocity;
	bool alive;
public:
	bool actionPossible(Vector2D pos);
	bool isAlive();
	
	/* Attribute Related functions */
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