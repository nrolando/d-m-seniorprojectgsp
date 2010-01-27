#include "BaseGameEntity.h"

int BaseGameEntity::entity_NextID = 0;

BaseGameEntity::BaseGameEntity(int ID, char _key, D3DXVECTOR3 pos, spriteSheet *ptr)
{
	setID(ID);
	name = GetNameOfEntity(ID);
	key = _key;
	sprInfo.POS = pos;
	sprInfo.ss_ptr = ptr;
	sprInfo.width = FRAME_WIDTH;
	sprInfo.height = FRAME_HEIGHT;
	state = anim = 0;
	speed = 2.0f;
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
	case SOLDIER_BOSS:
		maxHealth = health = 400;
		break;
	default:
		maxHealth = health = 50;
		break;
	};
}

BaseGameEntity::BaseGameEntity(std::string n)
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

void BaseGameEntity::setID(int val)
{
  ////make sure the val is equal to or greater than the next available ID
  //assert ( (val >= entity_NextID) && "<BaseGameEntity::SetID>: invalid ID");

  //entity_ID = val;
  //  
  //entity_NextID = entity_ID + 1;	
}

void BaseGameEntity::move(clock_t TIME)
{
	TIME = TIME/CLOCKS_PER_SEC;		//convert to seconds
	if(TIME < 1)
		TIME = 1;

	sprInfo.POS.x += vel.x*TIME;
	sprInfo.POS.y += vel.y*TIME;
	sprInfo.POS.z += vel.z*TIME;

	//keep player within boundaries
	if(sprInfo.POS.y >= YLIMIT_TOP)
	{
		sprInfo.POS.y = YLIMIT_TOP;
		vel.x = vel.y = vel.z = 0.0f;
	}
	if(sprInfo.POS.y <= YLIMIT_BOTTOM)
	{
		sprInfo.POS.y = YLIMIT_BOTTOM;
		vel.x = vel.y = vel.z = 0.0f;
	}
	if(sprInfo.POS.x <= -1500.0f)
	{
		sprInfo.POS.x = -1500.0f;
		vel.x = vel.y = vel.z = 0.0f;
	}
}

int BaseGameEntity::getDistance(D3DXVECTOR3 ePos,D3DXVECTOR3 pPos)
{
	double distance = sqrt(pow((ePos.x - pPos.x),2)+pow((ePos.y - pPos.y),2));
	return int(distance);
}