#ifndef PLAYER_H
#define PLAYER_H
#include "BaseGameEntity.h"

//these are for animation and stun length
#define STUNTIME		500
#define MAXSTUNFRAME	5
#define MAXIDLEFRAME	8
#define MAXWALKFRAME	12
#define MAXPUNCHFRAME	5
#define MAXKICKFRAME	7

class Player : public BaseGameEntity
{
private:
	//character attributes
	int special, pPower, kPower, sPower, lives;

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

	virtual void calcDrawRECT()
	{
		sprInfo.drawRect.left = anim * sprInfo.width;
		sprInfo.drawRect.right = sprInfo.drawRect.left + sprInfo.width;
		sprInfo.drawRect.top = state * sprInfo.height;
		sprInfo.drawRect.bottom = sprInfo.drawRect.top + sprInfo.height;
	}
	virtual void UpdateStat(int stat,int val);
	virtual void UpdateState(clock_t);

	//virtual void setImg(/* *DirectXSurface */);
	//virtual char getName();
};
#endif