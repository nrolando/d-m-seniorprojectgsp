#include <cassert>
#include <iostream>
#include "EnemyOwnedStates.h"
#include "Enemy.h"

Enemy::Enemy(int ID):BaseGameEntity(ID),
					 status(InRange),
					 CurrentState(Idle::Instance())
{

	state_frame = 0;
	rotated = false;
}

//possible bug: idk if passing a char array will get the c-str that its supposed to
Enemy::Enemy(int ID, char KEY, D3DXVECTOR3 pos, spriteSheet *ptr) 
:BaseGameEntity(ID, KEY, pos, ptr), status(InRange), CurrentState(Idle::Instance())
{
	state_frame = 0;
	rotated = false;
}

void Enemy::UpdateState(D3DXVECTOR3 playerPos)
{
	CurrentState->Execute(this,playerPos);

	//temp code: IM USING STATE/ANIM FROM BGE FOR RIGHT NOW.
	clock_t now = clock();
	switch(state)
	{
	case E_IDLE:
		if(now - aniFStart >= IDLEANIMATION)
		{
			if(anim < IDLEFRAME-1)
				anim++;
			else
				anim = 0;

			aniFStart = now;
		}
		break;
	case E_WALK:
		if(now - aniFStart >= CSWALKFRAMETIME)
		{
			if(anim < CSWALKFRAME-1)
				anim++;
			else
				anim = 0;

			aniFStart = now;
		}
		break;
	case E_ATTACK1:

		if(now - aniFStart >= ANIMATIONGAP)
		{
			if(anim < ATTACKFRAME-1)
				anim++;
			else
				anim = 1;

			aniFStart = now;
		}
		break;
	case E_DIE:		//this is playing for stun atm
		vel.x = vel.y = vel.z = 0.0f;

		if(now - aniFStart >= ANIMATIONGAP)
		{
			if(anim < CSWALKFRAME-1)
				anim++;

			aniFStart = now;
		}
		if(now - aniFStart >= STUNTIME)
			state = E_IDLE;
		break;
	}

	calcDrawRECT();
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
	sprInfo.hitBox.left = long(sprInfo.POS.x + 60);
	sprInfo.hitBox.right = sprInfo.hitBox.left + 68;
	sprInfo.hitBox.bottom  = sprInfo.hitBox.top + 88;

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

void Enemy::movement(char dir)
{
	clock_t now = clock();
	switch(dir)
	{
		case 'l':
			vel.x = -speed;
			vel.y = vel.z = 0;
			break;
		case 'd':
			vel.y = -speed;
			vel.x = vel.z = 0;
			break;
		case 'r':
			vel.x = speed;
			vel.y = vel.z = 0;
			break;
		case 'u':
			vel.y = speed; 
			vel.x = vel.z = 0;
			break;
		default:
			vel.x = vel.y = vel.z = 0;
			break;
	}
}