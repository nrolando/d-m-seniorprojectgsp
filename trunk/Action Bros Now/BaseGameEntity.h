#ifndef BASEGAMEENTITY_H
#define	BASEGAMEENTITY_H

#include "Status.h"
#include "common.h"
#include "spriteContainer.h"
#include <string>
#include <ctime>

#define ANIMATIONGAP	90
//all sprite sheets will have same frame width/height? if so, take out of eSprInfo and just use this
//right now, the player is instatiated with these values and the enemy reads its dimensions from file
//this is not being used
#define FRAME_WIDTH		128
#define FRAME_HEIGHT	128
#define E_FRAME_WIDTH	256
#define E_FRAME_HEIGHT	256

class Player;
class EntityManager;

class BaseGameEntity
{
private:
	int entity_ID;
	static int entity_NextID;
//----------------------------------------------------------
protected:
	//stat variables
	int health,maxHealth;
	//variables for the boss and player to be accessed by game
	int maxSpecial,special,sPower;
	//power for Enemy class
	int power;
	char key;
	//sprites info: pos, RECTs, image w/h, ss ptr,
	eSprInfo sprInfo;
	//set by getnameofentity(ID);
	std::string name;
	bool alive,miss;
	//entity velocity
	float speed;
	D3DXVECTOR3 vel;

//animations varibales
	clock_t stunStart, stunTime, aniFStart;
	//this is the state of the entity, and the current animation frame
	int state, anim;
	bool faceRight;		//keeps track of which direction the entity is facing
	bool passLvl;	//for player, allows him to move off the lvl
	bool tagged;

	//aggressive frames
	int hitFrames[3];
	//this variable keeps an aggressive frame from attacking more than once
	int lastAttFrame;
public:
	BaseGameEntity(int ID)
	{
		setID(ID); 
		name = GetNameOfEntity(ID);
		state = anim = 0;
	}
	//the constructor for enemies and bosses
	BaseGameEntity(int ID, char _key, D3DXVECTOR3 pos, spriteSheet *ptr);
	//the constructor for the player
	BaseGameEntity(std::string n);
	
	virtual ~BaseGameEntity(){}

	//Update Functions
	virtual void UpdateState(Player*,std::vector<BaseGameEntity*>) = 0;
	virtual void calcDrawRECT() = 0;
	virtual void UpdateStat(int, int) = 0;
	virtual void stun() = 0;
	virtual void stun(int) = 0;	//para: add-on to regular stun. extra stun time
	virtual void die() = 0;

	//move player according to velocity
	void move(clock_t TIME);	
	void setID(int val);

	//get methods
	std::string		getName()		{ return name; }
	spriteSheet*	getSSPtr()		{ return sprInfo.ss_ptr; }
	RECT			getSrc()		{ return sprInfo.drawRect; }
	D3DXVECTOR3		getPos()		{ return sprInfo.POS; }
	int				getHealth()		{ return health;}
	int				getMaxHealth()	{ return maxHealth;}
	int				getPower()		{ return power; }
	int				getWidth()		{ return sprInfo.width; }
	int				getHeight()		{ return sprInfo.height; }
	const int		ID()			{ return entity_ID;}
	eSprInfo		getDrawInfo()	{ return sprInfo; }
	int				getStatus()		{ return state;}
	int				getAnimFrame()	{ return anim; }
	int				getState()		{ return state; }
	int				getLastAttFrame(){ return lastAttFrame;}
	char			getKey()		{ return key; }
	bool			getPassLvl()	{ return passLvl; }
	int				getCurrHealth()	{ return int(float(health)/float(maxHealth)*100);}
	bool isFacing()					{ return faceRight;}
	bool isAlive()					{ return alive; }
	bool Missed()					{ return miss;}

	/*for the boss, quick and dirty*/
	int getSpecial() {return special;}
	int getMaxSpecial() {return maxSpecial;}
	int getsPower() {return sPower;}

	//set methods
	void missedAtk()				{ miss = true;}
	void resetAtk()					{ miss = false;}
	void setAlive(bool a)			{ alive = a; }
	void setFace(bool f)			{ faceRight = f; }
	void setHealth(int h)			{ health = h; }
	void setPower(int p)			{ power = p; }
	void setPassLvl(bool b)			{ passLvl = b; }
	void setSprInfo(eSprInfo esi)	{ sprInfo = esi; }
	void setPos(D3DXVECTOR3 p)		{ sprInfo.POS = p; }
	void setVel(D3DXVECTOR3 v)		{ vel = v; }
	void setSrc(RECT rect)			{ sprInfo.drawRect = rect; }
	void setSSPtr(spriteSheet *p)   { sprInfo.ss_ptr = p; }
	void setStatus(int s)			{ state = s;}//if(state != s){state=s; anim = 0;}}
	void resetHitFrames()			{ hitFrames[0] = hitFrames[1] = hitFrames[2] = 0;}
	void setLAF(int f)				{ lastAttFrame = f; }
	void setAnim(int a)				{ anim = a; }
	void setHitFrames(int n1, int n2, int n3)	{ hitFrames[0] = n1; hitFrames[1] = n2; hitFrames[2] = n3; }
	
	//check if the current animation frame is an aggressive frame and not equal to the last aggressive frame
	bool checkFrames()
	{
		if((anim == hitFrames[0] || anim == hitFrames[1] || anim == hitFrames[2]) && anim != lastAttFrame)
			return true;
		else
			return false;
	}
	void resetTimes()
	{
		aniFStart = clock();
	}

};

#endif