#ifndef BASEGAMEENTITY_H
#define	BASEGAMEENTITY_H

#include "Status.h"
#include "common.h"
#include "spriteContainer.h"
#include <string>
#include <ctime>

#define maxcharsize		50

#define ANIMATIONGAP	75
//all sprite sheets will have same frame width/height? if so, take out of eSprInfo and just use this
//right now, the player is instatiated with these values and the enemy reads its dimensions from file
//this is not being used
#define FRAME_WIDTH		128
#define FRAME_HEIGHT	128

class BaseGameEntity
{
private:
	int entity_ID;
	char key;
//----------------------------------------------------------
protected:
	//sprites info: pos, RECTs, image w/h, ss ptr,
	eSprInfo sprInfo;
	//set by getnameofentity(ID);
	std::string name;
	bool alive;
	int health;
	//entity velocity
	float speed;
	D3DXVECTOR3 vel;

//animations varibales
	clock_t now, stunStart, aniFStart;
	//this is the state of the entity, and the current animation frame
	int state, anim;
	int animTime, animStartTime;		//total time the animation will take
public:
	BaseGameEntity(int ID)
	{
		entity_ID = ID; 
		name = GetNameOfEntity(ID);
		state = anim = 0;
	}
	//the constructor for entities and bosses
	BaseGameEntity(int ID, char _key, D3DXVECTOR3 pos, spriteSheet *ptr)
	{
		entity_ID = ID;
		name = GetNameOfEntity(ID);
		key = _key;
		sprInfo.POS = pos;
		sprInfo.ss_ptr = ptr;
		sprInfo.width = FRAME_WIDTH;
		sprInfo.height = FRAME_HEIGHT;
		state = anim = 0;
		speed = 1.0f;
		vel.x = -speed;
		vel.y = 0.0f;
		vel.z = 0.0f;
		now = stunStart = aniFStart = 0;
	}
	//the constructor for the player
	BaseGameEntity(std::string n)
	{
		//player position will be initiated in game:initGame
		name = n;
		state = anim = 0;
		speed = 3.0f;
		vel.x = 0.0f;
		vel.y = 0.0f;
		vel.z = 0.0f;
		sprInfo.width = FRAME_WIDTH;
		sprInfo.height = FRAME_HEIGHT;
		now = stunStart = aniFStart = 0;
	}
	
	virtual ~BaseGameEntity(){}

	//Update Functions
	virtual void calcDrawRECT(int) = 0;
	virtual void UpdateStat(int, int) = 0;
	virtual void UpdateState(clock_t) = 0;

	//move player according to velocity
	void move()					{ sprInfo.POS += vel; }

	//get methods
	std::string		getName()		{ return name; }
	spriteSheet*	getSSPtr()		{ return sprInfo.ss_ptr; }
	RECT			getSrc()		{ return sprInfo.drawRect; }
	D3DXVECTOR3		getPos()		{ return sprInfo.POS; }
	int				getWidth()		{ return sprInfo.width; }
	int				getHeight()		{ return sprInfo.height; }
	eSprInfo		getDrawInfo()	{ return sprInfo; }

	//set methods
	void setSprInfo(eSprInfo esi)	{ sprInfo = esi; }
	void setPos(D3DXVECTOR3 p)		{ sprInfo.POS = p; }
	void setSrc(RECT rect)			{ sprInfo.drawRect = rect; }
	void setSSPtr(spriteSheet *p)   { sprInfo.ss_ptr = p; }

	int ID() {return entity_ID;}
	


};

#endif