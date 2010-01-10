#ifndef PLAYER_H
#define PLAYER_H
#include "BaseGameEntity.h"

//these are for animation and stun length
#define STUNTIME		500
//frame number count starts at 0
#define MAXIDLEFRAME	8
#define MAXWALKFRAME	12
#define MAXPUNCHFRAME	5
#define MAXKICKFRAME	7
#define MAXSTUNFRAME	0
#define MAXCOMBO1FRAME	20

#define MAXKICKANIMATION  50
#define MAXPUNCHANIMATION 30

class Player : public BaseGameEntity
{
private:
	//character attributes
	int special, maxSpecial, pPower, kPower, sPower, lives;
public:
	Player(std::string);
	bool isAlive();
	
	/* Attribute Related functions */
	void setState(int s)			{ state=s;}
	void takeDMG(int DMG)			{ health -= DMG;}
	void setSheetPtr(spriteSheet*);
	PlayerStates DoAction(char);

	/*Get Functions for Attributes*/
	int getSpecial()		{ return special;}
	int getMaxSpecial()     { return maxSpecial;}
	int getkPower()			{ return kPower;}
	int getsPower()			{ return sPower;}
	int getpPower()			{ return pPower;}
	std::string getName()	{ return name;}

	int UpdatePlayerState();
	virtual void calcDrawRECT(int);
	virtual void UpdateStat(int stat,int val);
	virtual void UpdateState() {};
};
#endif