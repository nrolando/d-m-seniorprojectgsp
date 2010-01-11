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
	clock_t now = clock();
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

	this->calcDrawRECT();
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

void Enemy::calcDrawRECT()
{
//**copied from player, will need adjustments**
	sprInfo.drawRect.left = anim * sprInfo.width;
	sprInfo.drawRect.right = sprInfo.drawRect.left + sprInfo.width;
	sprInfo.drawRect.top = state * sprInfo.height;
	sprInfo.drawRect.bottom = sprInfo.drawRect.top + sprInfo.height;

	//Enemy's hitBox for dmg verification
	sprInfo.hitBox.top  = long(sprInfo.POS.y);
	sprInfo.hitBox.left = long(sprInfo.POS.x + 85);
	sprInfo.hitBox.right = sprInfo.hitBox.left + 43;
	sprInfo.hitBox.bottom  = sprInfo.hitBox.top + 70;

	//Enemy's threatBox for dmg verification
	//while in DEBUG this will be shown
	if(state == PUNCH)
	{
		sprInfo.threatBox.top  = long(sprInfo.POS.y);
		sprInfo.threatBox.left = long(sprInfo.POS.x);
		sprInfo.threatBox.right = sprInfo.threatBox.left + 75;
		sprInfo.threatBox.bottom  = sprInfo.threatBox.top + 80;
	}
	else if(state == KICK)
	{
		sprInfo.threatBox.top  = long(sprInfo.POS.y);
		sprInfo.threatBox.left = long(sprInfo.POS.x);
		sprInfo.threatBox.right = sprInfo.threatBox.left + 90;
		sprInfo.threatBox.bottom  = sprInfo.threatBox.top + 80;
	}
	else
	{
		sprInfo.threatBox.top  = -8880;
		sprInfo.threatBox.left = -8880;
		sprInfo.threatBox.right = -8880;
		sprInfo.threatBox.bottom = -8880;
	}
}