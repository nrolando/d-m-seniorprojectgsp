#ifndef BASEGAMEENTITY_H
#define	BASEGAMEENTITY_H

#include "Status.h"
#include "common.h"
#include "spriteContainer.h"
#include <string>
#include <ctime>

#define maxcharsize		50
#define ANIMATIONGAP	150

class BaseGameEntity
{
private:
	int entity_ID;
	char key;
//----------------------------------------------------------
protected:
	Vector2D POS;
	std::string name;		//set by getnameofentity(ID);
	D3DXVECTOR3 vel;

//animations varibales
	clock_t now, stunStart, aniFStart;
	//variables we talked about tues 12/1/09 to figure out m_src
	int width, height;	//width and height of frames
	RECT m_src;	//source of sprite surface, determined by state/animation, w/h
	//this is the state of the entity, and the current animation frame
	int state, anim;

//THIS IS ONLY FOR ENTITIES, Player HAS ITS OWN POINTER
	Sprite *m_ptr;
public:
	BaseGameEntity(int ID)
	{
		entity_ID = ID; 
		name = GetNameOfEntity(ID);
		state = anim = 0;
	}
	//the constructor for enemies and bosses
	BaseGameEntity(int ID, char _key, Vector2D pos, Sprite *ptr, int w, int h)
	{
		entity_ID = ID;
		name = GetNameOfEntity(ID);
		key = _key;
		POS = pos;
		m_ptr = ptr;
		state = anim = 0;
		width = w;
		height = h;
		vel.x = -5.0f;
		vel.y = 0.0f;
		vel.z = 0.0f;
		now = stunStart = aniFStart = 0;
	}
	//the constructor for the player
	BaseGameEntity(std::string n)
	{
		name = n;
		POS.x = -1200.0f;
		POS.y = 0.0f;
		state = anim = 0;
		vel.x = 5.0f;
		vel.y = 5.0f;
		vel.z = 0.0f;
		now = stunStart = aniFStart = 0;
	}
	
	virtual ~BaseGameEntity(){}

	void calcRECT() { state = anim = 0; width = 89; height = 70; }

	virtual void UpdateStat(int stat, int val) = 0;
	virtual void UpdateState(clock_t) = 0;
	//virtual void setImg(/* *DirectXSurface */);
	void move()					{ POS.x += vel.x; POS.y += vel.y; }

	//get methods
	std::string getName()	{ return name; }
	Sprite* getSpritePtr()	{ return m_ptr; }
	RECT getSrc()			{ return m_src; }
	Vector2D getPos()		{ return POS; }
	int getWidth()			{ return width; }
	int getHeight()			{ return height; }

	//set methods
	void setSrc(RECT rect)	{ m_src = rect; }
	void setSpritePtr(Sprite *ptr)	{ m_ptr = ptr; }

	int ID() {return entity_ID;}
	


};

#endif