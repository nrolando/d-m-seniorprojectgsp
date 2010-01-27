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
	speed = 1.0f;
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

int BaseGameEntity::getDistance(D3DXVECTOR3 ePos,D3DXVECTOR3 pPos)
{
	double distance = sqrt(pow((ePos.x - pPos.x),2)+pow((ePos.y - pPos.y),2));
	return int(distance);
}