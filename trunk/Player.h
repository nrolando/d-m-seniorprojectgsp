#ifndef PLAYER_H
#define PLAYER_H
#include "BaseGameEntity.h"

//these are for animation and stun length
#define STUNTIME		500
#define ANIMATIONGAP	30
#define MAXSTUNFRAME	5
#define MAXIDLEFRAME	5
#define MAXWALKFRAME	5


class Player : protected BaseGameEntity
{
private:
	//character attributes
	int health,special,
		pPower,kPower,sPower, lives;
	eSprInfo *sprInfo;

	//clock vars for animations & stun timers
//!!!!!!!!init these to 0!!!!!!!!!!!!!!!!!!!!!!!!
	clock_t now, stunStart, aniFStart;

	//enum for the player's state
	//init to idle
	int currState;

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