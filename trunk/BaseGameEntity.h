#ifndef BASEGAMEENTITY_H
#define	BASEGAMEENTITY_H

#include "Status.h"
#include "common.h"
#include <string>
#include <ctime>

#define maxcharsize		50

class BaseGameEntity
{
private:
	int entity_ID;
	char key;
//variables we talked about tues 12/1/09 to figure out m_src
	int width, height;
	int state, anim;
//----------------------------------------------------------
protected:
	Vector2D POS;
	std::string name;		//set by getnameofentity(ID);
	RECT m_src;	//source of sprite surface, determined by state/animation, w/h
	
	Sprite *m_ptr;
public:
	BaseGameEntity(int ID) {entity_ID = ID; 
							name = GetNameOfEntity(ID);}
	BaseGameEntity(int ID, char _key, Vector2D pos, Sprite *ptr)
	{
		entity_ID = ID;
		name = GetNameOfEntity(ID);
		key = _key;
		POS = pos;
		m_ptr = ptr;
	}
	
	virtual ~BaseGameEntity(){}

	void calcRECT() { state = anim = 0; width = 89; height = 70; }

	virtual void UpdateStat(int stat, int val) = 0;
	virtual void UpdateState() = 0;
	//virtual void setImg(/* *DirectXSurface */);

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