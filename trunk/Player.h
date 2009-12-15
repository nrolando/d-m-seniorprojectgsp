#ifndef PLAYER_H
#define PLAYER_H
#include "BaseGameEntity.h"

class Player : protected BaseGameEntity
{
private:
	//character attributes
	int health,special,
		pPower,kPower,sPower, lives;
	eSprInfo *sprInfo;

	//character's speed/movement 
	D3DXVECTOR3 POS;
	D3DXVECTOR3 vel;
	bool alive;
public:
	Player(std::string);
	bool actionPossible(D3DXVECTOR3);
	bool isAlive();
	
	/* Attribute Related functions */
	void DoAction(char);
	void setState(int s) {state=s;}
	void setSheetPtr(spriteSheet*);
	void takeDMG(int DMG) {health -= DMG;}
	
	/*Get Functions for Attributes*/
	int getHealth() {health;}
	int getSpecialMeter() {special;}
	int getkPower() {kPower;}
	int getsPower() {sPower;}
	int getpPower() {pPower;}
	std::string getName()	{ return name;}
	

	RECT getCBox() {return sprInfo->cBox;}
	eSprInfo* getDrawInfo() {return sprInfo;}

	virtual void UpdateStat(int stat,int val);
	virtual void UpdateState(clock_t);

	//virtual void setImg(/* *DirectXSurface */);
	//virtual char getName();
};
#endif