#include <cassert>
#include <iostream>
#include "EnemyOwnedStates.h"
#include "Enemy.h"

Enemy::Enemy(char entity_type,Vector2D pos):BaseGameEntity(entity_type,pos),
					 status(InRange),
					 CurrentState(Idle::Instance())
{}

void increaseHealth(int h)
{}
void Enemy::UpdateState()
{
	CurrentState->Execute(this);
}

void Enemy::ChangeState(State<Enemy>* pNewState)
{
	CurrentState->Exit(this);
	CurrentState = pNewState;
	CurrentState->Enter(this);
}

void Enemy::UpdateStat(int stat, int val)
{
	switch(stat)
	{
		case 0:
			health += val;
			break;
		default:
			printf("Sorry incorrect stat addition");
			break;
	}
}

bool Enemy::isAlive()
{
	if(health > 0 && health <= 200)
		return true;
	else
		return false;
}