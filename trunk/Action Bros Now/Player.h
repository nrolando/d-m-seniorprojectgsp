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


class Player : public BaseGameEntity
{
private:
	//character attributes
	int special, pPower, kPower, sPower, lives;
public:
	Player(std::string);

	bool isAlive();
	
	/* Attribute Related functions */
	PlayerStates DoAction(char);
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

	int UpdatePlayerState();
	virtual void calcDrawRECT(int);
	virtual void UpdateStat(int stat,int val);
	virtual void UpdateState() {};
};
#endif