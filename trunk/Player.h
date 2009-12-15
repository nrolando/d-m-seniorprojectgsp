#ifndef PLAYER_H
#define PLAYER_H
#include "BaseGameEntity.h"

class Player : protected BaseGameEntity
{
private:
	//character attributes
	int health,special,
		pPower,kPower,sPower, lives;
	eSprInfo sprInfo;
	//character's speed/movement 
	D3DXVECTOR3 POS;
	bool alive;
public:
	Player(int ID);
	bool actionPossible(D3DXVECTOR3);
	bool isAlive();
	
	/* Attribute Related functions */
	void setState(int);
	void takeDMG(float DMG);
	
	/*Get Functions for Attributes*/
	int getHealth() {health;}
	int getSpecialMeter() {special;}
	int getkPower() {kPower;}
	int getsPower() {sPower;}
	int getpPower() {pPower;}
	eSprInfo getDrawInfo() {return sprInfo;}
	RECT getCBox() {return sprInfo.cBox;}

	virtual void UpdateStat(int stat,int val);
	virtual void UpdateState();

	//virtual void setImg(/* *DirectXSurface */);
	//virtual char getName();
};
#endif