#ifndef PLAYER_H
#define PLAYER_H
#include "BaseGameEntity.h"

//these are for animation and stun length
#define STUNTIME		500
#define MAXSTUNFRAME	5
#define MAXIDLEFRAME	9
#define MAXWALKFRAME	13
#define MAXPUNCHFRAME	6
#define MAXKICKFRAME	8

class Player : public BaseGameEntity
{
private:
	//character attributes
	int special, pPower, kPower, sPower, lives;

public:
	Player(std::string);

	bool actionPossible(char);
	bool isAlive();
	
	/* Attribute Related functions */
	void DoAction(char,InputManager2*);
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

	void UpdatePlayerState(clock_t,InputManager2*);
	virtual void calcDrawRECT(int);
	virtual void UpdateStat(int stat,int val);
	virtual void UpdateState(clock_t) {};

	//virtual void setImg(/* *DirectXSurface */);
	//virtual char getName();
};
#endif