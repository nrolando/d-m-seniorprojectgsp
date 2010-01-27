#include <cassert>
#include <iostream>
#include "EnemyOwnedStates.h"
#include "Enemy.h"

Enemy::Enemy(int ID):BaseGameEntity(ID),
					 status(InRange),
					 CurrentState(Idle::Instance())
{
	faceRight = false;
}

//possible bug: idk if passing a char array will get the c-str that its supposed to
Enemy::Enemy(int ID, char KEY, D3DXVECTOR3 pos, spriteSheet *ptr) 
:BaseGameEntity(ID, KEY, pos, ptr), status(InRange), CurrentState(Idle::Instance())
{
	if(KEY == SOLDIER1)
		faceRight = false;
	else
		faceRight = false;
}

void Enemy::UpdateState(Player *p)
{
	if(state != E_STUN)
		CurrentState->Execute(this,p);

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
			{
				anim = 0;
				lastAttFrame = -1;
			}

			aniFStart = now;
		}
		break;
	case E_DIE:
		if(now - aniFStart >= ANIMATIONGAP)
		{
			if(anim < CSWALKFRAME-1)
				anim++;

			aniFStart = now;
		}
		if(now - aniFStart >= STUNTIME)
			state = E_IDLE;
		break;
	case E_STUN:
		//if has been long enough
		if(now - stunStart >= stunTime)
		{
			//switch to first fram of idle state
			state = E_IDLE;
			anim = 0;
			aniFStart = now;
		}
		//if still stunned and time to switch frame of animation
		else if(now - aniFStart >= ANIMATIONGAP)
		{
			//loop to the beginning of the animation
			if(anim >= STUNFRAME-1)
				anim = 0;
			//advance 1 frame
			else
				anim++;
			aniFStart = now;
		}
		break;
	}

	calcDrawRECT();
}

void Enemy::ChangeState(State<Enemy,Player>* pNewState)
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
			if(health < 1)
				alive = false;
			break;
		default:
			printf("Sorry incorrect stat addition");
			break;
	}
}

void Enemy::calcDrawRECT()
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

//ENEMY HITBOX FOR DAMAGE VERIFICATION
	switch(this->key)
	{
	case SOLDIER1:
		sprInfo.hitBox.top  = long(sprInfo.POS.y - 66);
		sprInfo.hitBox.left = long(sprInfo.POS.x + 40);
		sprInfo.hitBox.right = sprInfo.hitBox.left + 43;
		sprInfo.hitBox.bottom  = sprInfo.hitBox.top - 62;
		break;
	default:
		break;
	};

	//Enemy's threatBox for dmg verification
	//while in DEBUG this will be shown
	//PLEASE NOTE: THIS IS ALL FOR RIGHT FACING ONLY! LEFT FACE IS NOT SET UP YET. ALSO, IM NOT SURE
	//IF DONNIE PLANS ON SHARING THE CURRENT ENEMY STATES BETWEEN ALL ENEMY TYPES, OR EACH ENEMY HAVING ITS
	//OWN SET OF STATES, SO IM PUTTING THIS INSIDE A SWITCH TO BE SAFE
	switch(this->key)
	{
	case SOLDIER1:
		if(state == E_ATTACK1)
		{
			if(faceRight)
			{
				sprInfo.threatBox.top  = long(sprInfo.POS.y - 79);
				sprInfo.threatBox.left = long(sprInfo.POS.x + 71);
				sprInfo.threatBox.right = sprInfo.threatBox.left + 56;
				sprInfo.threatBox.bottom  = sprInfo.threatBox.top - 21;
			}
			else
			{
				sprInfo.threatBox.top  = long(sprInfo.POS.y - 79);
				sprInfo.threatBox.left = long(sprInfo.POS.x + 1);
				sprInfo.threatBox.right = sprInfo.threatBox.left + 56;
				sprInfo.threatBox.bottom  = sprInfo.threatBox.top - 21;
			}
		}
		else
		{	//make threatbox off world
			sprInfo.threatBox.top  = -8880;
			sprInfo.threatBox.left = -8880;
			sprInfo.threatBox.right = -8880;
			sprInfo.threatBox.bottom = -8880;
		}
		break;
	default:
		break;
	};
}

void Enemy::movement(char dir)
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

void Enemy::stun()
{
	//min + rand() % max - min + 1
	stunTime = 200 + rand() % 301;
	stunStart = clock();
	state = E_STUN;
	this->setVel(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}