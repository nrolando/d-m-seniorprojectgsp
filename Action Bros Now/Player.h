#ifndef PLAYER_H
#define PLAYER_H
#include "BaseGameEntity.h"

//these are for animation and stun length
#define STUNTIME		500
#define MAXSTUNFRAME	5
#define MAXIDLEFRAME	5
#define MAXWALKFRAME	5
//#define SPRITE_HEIGHT 128
//#define SPRITE_WIDTH 128


class Player : public BaseGameEntity
{
private:
	//character attributes
	int health,special,
		pPower,kPower,sPower, lives;

	bool alive;
public:
	Player();
	Player(std::string);

	bool actionPossible(char);
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

//will be moved/modified
	//void initSprInfo(int, int, D3DXVECTOR3);

	virtual void UpdateStat(int stat,int val);
	virtual void UpdateState(clock_t);

	//virtual void setImg(/* *DirectXSurface */);
	//virtual char getName();
};
#endif