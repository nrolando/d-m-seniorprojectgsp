#include <cassert>
#include <iostream>
#include "EnemyOwnedStates.h"
#include "Enemy.h"

Enemy::Enemy(int ID):BaseGameEntity(ID),
					 status(InRange),
					 CurrentState(Idle::Instance())
{
	state_frame = 0;
}

//possible bug: idk if passing a char array will get the c-str that its supposed to
Enemy::Enemy(int ID, char KEY, D3DXVECTOR3 pos, spriteSheet *ptr)
			:BaseGameEntity(ID, KEY, pos, ptr),
							 status(InRange),
							 CurrentState(Idle::Instance())
{
	state_frame = 0;
}

void increaseHealth(int h);

void Enemy::UpdateState(clock_t ct)
{
	CurrentState->Execute(this);

	state = AN_ENEMY_STATE;

//temp code: this is better code for you to work with. this the basic idea. of course things will be changed
	now = clock();
	switch(state)
	{
	case AN_ENEMY_STATE:
		//set the animation frame for this state
		state_frame = 0;

		if(now - aniFStart >= ANIMATIONGAP)
		{
			this->calcDrawRECT(state);

			if(anim < 3)
				anim++;
			else
				anim = 0;

			aniFStart = now;
		}
		break;
	};
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