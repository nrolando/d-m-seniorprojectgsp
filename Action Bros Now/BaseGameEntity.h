#ifndef BASEGAMEENTITY_H
#define	BASEGAMEENTITY_H

#include "Status.h"
#include "common.h"
#include "spriteContainer.h"
#include <string>
#include <ctime>

#define maxcharsize		50
#define ANIMATIONGAP	90
//all sprite sheets will have same frame width/height? if so, take out of eSprInfo and just use this
//right now, the player is instatiated with these values and the enemy reads its dimensions from file
//this is not being used
#define FRAME_WIDTH		128
#define FRAME_HEIGHT	128

class Player;

class BaseGameEntity
{
private:
	int entity_ID;
	static int entity_NextID;
//----------------------------------------------------------
protected:
	//stat variables
	int health,maxHealth;
	//power for Enemy class
	int power;

	char key;
	//sprites info: pos, RECTs, image w/h, ss ptr,
	eSprInfo sprInfo;
	//set by getnameofentity(ID);
	std::string name;
	bool alive;
	//entity velocity
	float speed;
	D3DXVECTOR3 vel;

//animations varibales
	clock_t stunStart, stunTime, aniFStart;
	//this is the state of the entity, and the current animation frame
	int state, anim;
	bool faceRight;		//keeps track of which direction the entity is facing

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
	BaseGameEntity(int ID, char _key, D3DXVECTOR3 pos, spriteSheet *ptr)
	{
		setID(ID);
		name = GetNameOfEntity(ID);
		key = _key;
		sprInfo.POS = pos;
		sprInfo.ss_ptr = ptr;
		sprInfo.width = FRAME_WIDTH;
		sprInfo.height = FRAME_HEIGHT;
		state = anim = 0;
		speed = 1.0f;
		health = maxHealth = 100;
		vel.x = 0.0f;
		vel.y = 0.0f;
		vel.z = 0.0f;
		stunStart = 0;
		aniFStart = clock();
		switch(key)
		{
		case SOLDIER1:
			maxHealth = health = 60;
			break;
		default:
			maxHealth = health = 50;
			break;
		};
	}
	//the constructor for the player
	BaseGameEntity(std::string n)
	{
		//player position will be initiated in game:initGame
		name = n;
		maxHealth = health = 100;
		state = anim = 0;
		speed = 3.0f;
		vel.x = 0.0f;
		vel.y = 0.0f;
		vel.z = 0.0f;
		sprInfo.width = FRAME_WIDTH;
		sprInfo.height = FRAME_HEIGHT;
		stunStart = 0;
		aniFStart = clock();
		faceRight = true;
	}
	
	virtual ~BaseGameEntity(){}

	//Update Functions
	virtual void calcDrawRECT() = 0;
	virtual void UpdateStat(int, int) = 0;
	virtual void UpdateState(D3DXVECTOR3) = 0;

	//move player according to velocity
	void move(clock_t TIME)
	{
		TIME = TIME/CLOCKS_PER_SEC;		//convert to seconds
		if(TIME < 1)
			TIME = 1;

		sprInfo.POS.x += vel.x*TIME;
		sprInfo.POS.y += vel.y*TIME;
		sprInfo.POS.z += vel.z*TIME;
	}

	
	void setID(int val);
	virtual void stun() = 0;

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
	int getAnimFrame()				{ return anim; }
	int getLastAttFrame()			{ return lastAttFrame; }
	int getDistance(D3DXVECTOR3,D3DXVECTOR3);		


	//set methods
	bool isAlive()					{ return alive; }
	void setPower(int p)			{ power = p; }
	void setSprInfo(eSprInfo esi)	{ sprInfo = esi; }
	void setPos(D3DXVECTOR3 p)		{ sprInfo.POS = p; }
	void setVel(D3DXVECTOR3 v)		{ vel = v; }
	void setSrc(RECT rect)			{ sprInfo.drawRect = rect; }
	void setSSPtr(spriteSheet *p)   { sprInfo.ss_ptr = p; }
	void setState(int s)			{ state = s; }
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