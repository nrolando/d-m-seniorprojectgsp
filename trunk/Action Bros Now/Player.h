#ifndef PLAYER_H
#define PLAYER_H
#include "BaseGameEntity.h"
#include "soundManager.h"

class Player : public BaseGameEntity
{
private:
	//character attributes
	int special, maxSpecial, lives;
	int score;
public:
	Player(std::string);
	
	/* Attribute Related functions */
	void setScore(int s)			{ score = s; }
	//MIKE"S CHANGE: the if statement stops idle animation from constanly restarting when a wall is hit
	void setState(int s)			{ if(state != s){state=s; anim = 0;}}
	void takeDMG(int DMG)			{ health -= DMG;}
	void setSheetPtr(spriteSheet*);
	void DoAction(char);

	/*Get Functions for Attributes*/
	int getScore()			{ return score; }
	int getSpecial()		{ return special;}
	int getMaxSpecial()     { return maxSpecial;}
	int getDmg();
	std::string getName()	{ return name;}

	int UpdatePlayerState();
	void addScore(int s)			{ score += s; }
	virtual void calcDrawRECT();
	virtual void UpdateStat(int stat,int val);
	virtual void UpdateState(Player*) {};
	virtual void stun();
	virtual void die();
};
#endif