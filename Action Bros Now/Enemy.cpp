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

void Enemy::UpdateState()
{
	CurrentState->Execute(this);

	//temp code: IM USING STATE/ANIM FROM BGE FOR RIGHT NOW. IDK HOW ENEMYOWNEDSTATES AND THE STATE CLASSES WORK
	now = clock();
	switch(state)
	{
	case CS_WALK:
		//just make them move left for now
		vel.x = -speed;
		vel.y = vel.z = 0.0f;
		//set the animation frame for this state
		state_frame = 0;

		if(now - aniFStart >= ANIMATIONGAP)
		{
			this->calcDrawRECT(state);

			if(anim < CSWALKFRAME)
				anim++;
			else
				anim = 0;

			aniFStart = now;
		}
		break;
	case CS_DIE:		//this is playing for stun atm
		vel.x = vel.y = vel.z = 0.0f;

		if(now - aniFStart >= ANIMATIONGAP)
		{
			this->calcDrawRECT(state);

			if(anim < CSWALKFRAME)
				anim++;
			else
				anim = 0;

			aniFStart = now;
		}
		if(now - aniFStart >= STUNTIME)
			state = CS_WALK;
		break;
	};
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