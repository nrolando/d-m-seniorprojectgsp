#include "Boss.h"
#include "Enemy.h"
#include "EnemyOwnedStates.h"
#include "BossOwnedStates.h"

Boss::Boss(int ID):BaseGameEntity(ID)
{}

//possible bug: idk if passing a char array will get the c-str that its supposed to
Boss::Boss(int ID, char KEY, D3DXVECTOR3 pos, spriteSheet *ptr)
			:BaseGameEntity(ID, KEY, pos, ptr), CurrentState(Stationary::Instance())
{
	special = maxSpecial = 150;
	sPower = 50;
	AnimFinished = true;
}

bool Boss::isAlive()
{
	return alive;
}

void Boss::ChangeState(State<Boss, Player>* pNewState)
{
	CurrentState->Exit(this);
	CurrentState = pNewState;
	CurrentState->Enter(this);
}

void Boss::UpdateStat(int stat, int val)
{
	switch(stat)
	{
		case 0:
			health += val;
			break;
		case 1:
			special += val;
			break;
		default:
			printf("Sorry incorrect stat addition");
			break;
	}
}

void Boss::UpdateState(Player *p,std::vector<BaseGameEntity*> e)
{
	CurrentState->Execute(this,p,e);

	//temp code: IM USING STATE/ANIM FROM BGE FOR RIGHT NOW.
	clock_t now = clock();
	switch(state)
	{
		case SB_IDLE:
			if(now - aniFStart >= SBIDLEANIMATION)
			{
				if(anim < SB_IDLE_FRAME-1)
				{
					AnimFinished = false;
					anim++;
				}
				else
				{
					AnimFinished = true;
					anim = 0;
				}

				aniFStart = now;
			}
			break;
		case SB_WALK:
			if(now - aniFStart >= SBWALKANIMATION)
			{
				if(anim < SB_WALK_FRAME-1)
				{
					AnimFinished = false;
					anim++;
				}
				else
				{
					AnimFinished = true;
					anim = 0;
				}

				aniFStart = now;
			}
			break;
		case SB_RUN:
			if(now - aniFStart >= SBRUNANIMATION)
			{
				if(anim < SB_RUN_FRAME-1)
				{
					AnimFinished = false;
					anim++;
				}
				else
				{
					AnimFinished = true;
					anim = 0;
				}

				aniFStart = now;
			}
			break;
		case SB_KICK:
			if(now - aniFStart >= SBKICKANIMATION)
			{
				if(anim < SB_KICK_FRAME-1)
				{
					AnimFinished = false;
					anim++;
				}
				else
				{
					AnimFinished = true;
					anim = 0;
				}

				aniFStart = now;
			}
			break;
		case SB_SLASH:
			if(now - aniFStart >= SBSLASHANIMATION)
			{
				if(anim < SB_WALK_FRAME-1)
				{
					AnimFinished = false;
					anim++;
				}
				else
				{
					AnimFinished = true;
					anim = 0;
				}

				aniFStart = now;
			}
			break;
		case SB_STUN:
			if(now - aniFStart >= GENERALANIMATION)
			{
				if(anim < SB_WALK_FRAME-1)
				{
					AnimFinished = false;
					anim++;
				}
				else
				{
					AnimFinished = true;
					anim = 0;
				}

				aniFStart = now;
			}
			break;
		case SB_DIE:
			if(now - aniFStart >= GENERALANIMATION)
			{
				if(anim < SB_WALK_FRAME-1)
					anim++;
				else
					anim = 0;

				aniFStart = now;
			}
			break;
		case SB_TAUNT:
			if(now - aniFStart >= GENERALANIMATION)
			{
				if(anim < SB_WALK_FRAME-1)
				{
					AnimFinished = false;
					anim++;
				}
				else
				{
					AnimFinished = true;
					anim = 0;
				}

				aniFStart = now;
			}
			break;
		
	};

	calcDrawRECT();
}

void Boss::movement(char dir)
{
	clock_t now = clock();
	switch(dir)
	{
		case 'l':
			faceRight = false;
			vel.x = -speed;
			vel.y = vel.z = 0;
			break;
		case 'd':
			vel.y = -speed;
			vel.x = vel.z = 0;
			break;
		case 'r':
			faceRight = true;
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

void Boss::calcDrawRECT()
{
	int state_frame;

	switch(this->key)
	{
	case SOLDIER1:
		if(faceRight)
			state_frame = state;
		else
			state_frame = state + SOLDIER1STATES;
		break;
	default:
		state_frame = state;
	};

	//source rect to be drawn
	sprInfo.drawRect.left = anim * sprInfo.width;
	sprInfo.drawRect.right = sprInfo.drawRect.left + sprInfo.width;
	sprInfo.drawRect.top = state_frame * sprInfo.height;
	sprInfo.drawRect.bottom = sprInfo.drawRect.top + sprInfo.height;
}

void Boss::stun()
{
	//min + rand() % max - min + 1
	stunTime = 200 + rand() % 301;
	stunStart = clock();
	state = E_STUN;
	this->setVel(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

void Boss::stun(int num)
{
	//min + rand() % max - min + 1
	stunTime = (200 + rand() % 301) + num;
	stunStart = clock();
	state = E_STUN;
	this->setVel(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}