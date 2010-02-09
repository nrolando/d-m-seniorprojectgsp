#ifndef PLAYER_H
#define PLAYER_H
#include "BaseGameEntity.h"
#include "soundManager.h"

#define MAXHEALTH		100

class Enemy;

class Player : public BaseGameEntity
{
private:
	//character attributes
	int lives;
	int score;
	bool stunned;
public:
	Player(std::string);
	
	/* Attribute Related functions */
	void setScore(int s)			{ score = s; }
	void setLives(int l)			{ lives = l; }
	//MIKE"S CHANGE: the if statement stops idle animation from constanly restarting when a wall is hit
	void setState(int s)			{ if(state != s){state=s; anim = 0;}}
	void takeDMG(int DMG)			{ health -= DMG;}
	void setSheetPtr(spriteSheet*);
	void DoAction(char);
	void decLives()					{ lives--; }

	/*Get Functions for Attributes*/
	bool isStunned()		{ return stunned;}
	int getScore()			{ return score; }
	int getLives()			{ return lives; }
	int getSpecial()		{ return special;}
	int getMaxSpecial()     { return maxSpecial;}
	int getDmg();
	std::string getName()	{ return name;}

	int UpdatePlayerState();
	void addScore(int s)			{ score += s; }
	virtual void calcDrawRECT();
	virtual void UpdateStat(int stat,int val); 
	virtual void UpdateState(Player*,std::vector<BaseGameEntity*>) {};
	virtual void stun();
	virtual void stun(int);
	virtual void die();
};
#endif