#include <cassert>
#include <iostream>
#include "EnemyOwnedStates.h"
#include "Enemy.h"

Enemy::Enemy(int ID):BaseGameEntity(ID),
					 status(InRange),
					 CurrentState(Idle::Instance())
{}

//possible bug: idk if passing a char array will get the c-str that its supposed to
Enemy::Enemy(int ID, char KEY, D3DXVECTOR3 pos, spriteSheet *ptr, int w, int h)
			:BaseGameEntity(ID, KEY, pos, ptr, w, h),
							 status(InRange),
							 CurrentState(Idle::Instance())
{}

void increaseHealth(int h);

void Enemy::UpdateState(clock_t ct)
{
	CurrentState->Execute(this);

//temp code: this is better code for you to work with. this the basic idea. of course things will be changed
	now = clock();
	if(now - aniFStart >= ANIMATIONGAP)
	{
		this->calcDrawRECT();

		if(anim == 5)
			anim = 0;
		else
			anim++;

		aniFStart = now;
	}
	this->move();
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