#ifndef PLAYER_H
#define PLAYER_H
#include "BaseGameEntity.h"

class Player : public BaseGameEntity
{
private:
	//character attributes
	int special, maxSpecial, lives;
public:
	Player(std::string);
	
	/* Attribute Related functions */
	void setState(int s)			{ state=s;}
	void takeDMG(int DMG)			{ health -= DMG;}
	void setSheetPtr(spriteSheet*);
	void DoAction(char);

	/*Get Functions for Attributes*/
	int getSpecial()		{ return special;}
	int getMaxSpecial()     { return maxSpecial;}
	int getDmg();
	std::string getName()	{ return name;}

	int UpdatePlayerState();
	virtual void calcDrawRECT();
	virtual void UpdateStat(int stat,int val);
	virtual void UpdateState(D3DXVECTOR3 pPos) {};
	virtual void stun();
};
#endif