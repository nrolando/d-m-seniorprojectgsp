#include "BaseGameEntity.h"

int BaseGameEntity::entity_NextID = 0;

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