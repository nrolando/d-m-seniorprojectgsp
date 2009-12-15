#include <cassert>
#include <iostream>
#include "EnemyOwnedStates.h"
#include "Enemy.h"

Enemy::Enemy(std::string name):BaseGameEntity(name),
					 status(InRange),
					 CurrentState(Idle::Instance())
{}

//possible bug: idk if passing a char array will get the c-str that its supposed to
Enemy::Enemy(std::string name, D3DXVECTOR3 pos)
			:BaseGameEntity(name, pos),
			 status(InRange),
			 CurrentState(Idle::Instance())
{}

void increaseHealth(int h);

void Enemy::UpdateState(clock_t time)
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