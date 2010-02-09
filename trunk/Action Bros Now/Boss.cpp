#include "Boss.h"
#include "Player.h"
#include "EnemyOwnedStates.h"
#include "BossOwnedStates.h"

Boss::Boss(int ID):BaseGameEntity(ID)
{}

//possible bug: idk if passing a char array will get the c-str that its supposed to
Boss::Boss(int ID, char KEY, D3DXVECTOR3 pos, spriteSheet *ptr)
			:BaseGameEntity(ID, KEY, pos, ptr), CurrentState(Stationary::Instance())
{
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
			if(health <= 0)
			{
				alive = false;
				health = 0;
			}
			break;
		case 1:
			special += val;
			break;
		case 2:
			speed = float(val);
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
					anim++;
				}
				else
				{
					lastAttFrame = -1;
					anim = 0;
				}

				aniFStart = now;
			}
			break;
		case SB_SLASH:
			if(now - aniFStart >= SBSLASHANIMATION)
			{
				if(anim < SB_SLASH_FRAME-1)
				{
					anim++;
				}
				else
				{
					lastAttFrame = -1;
					anim = 0;
				}

				aniFStart = now;
			}
			break;
		case SB_DIE:
			if(this->getCurrHealth() == 0)
			{
				if(now - aniFStart >= GENERALANIMATION)
				{
					if(anim < SB_DEATH_FRAME-1)
						anim++;
					else
						alive = false;

					aniFStart = now;
				}
			}
			else
			{
				if(now - aniFStart >= GENERALANIMATION)
				{
					if(anim < SB_STUN_FRAME-1)
						anim++;
					else
						anim = 0;

					aniFStart = now;
				}
			}
			break;
		case SB_TAUNT:
			if(now - aniFStart >= GENERALANIMATION)
			{
				if(anim < SB_TAUNT_FRAME-1)
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

	if(p->getDrawInfo().hitBox.left > this->getDrawInfo().hitBox.left)
		faceRight = true;
	else
		faceRight = false;

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
			//vel.y = vel.z = 0;
			break;
		case 'd':
			vel.y = -speed;
			//vel.x = vel.z = 0;
			break;
		case 'r':
			faceRight = true;
			vel.x = speed;
			//vel.y = vel.z = 0;
			break;
		case 'u':
			vel.y = speed;
			//vel.x = vel.z = 0;
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
	case SOLDIER_BOSS:
		if(faceRight)
			state_frame = state;
		else
			state_frame = state + SOLDIERBOSSSTATES;
		break;
	default:
		state_frame = state;
	};

	//source rect to be drawn
	sprInfo.drawRect.left = anim * sprInfo.width;
	sprInfo.drawRect.right = sprInfo.drawRect.left + sprInfo.width;
	sprInfo.drawRect.top = state_frame * sprInfo.height;
	sprInfo.drawRect.bottom = sprInfo.drawRect.top + sprInfo.height;

	switch(this->key)
	{
		case SOLDIER_BOSS:
			sprInfo.hitBox.top  = long(sprInfo.POS.y - 181);
			sprInfo.hitBox.left = long(sprInfo.POS.x + 101);
			sprInfo.hitBox.right = sprInfo.hitBox.left + 45;
			sprInfo.hitBox.bottom  = sprInfo.hitBox.top - 75;
			if(state == SB_KICK || state == SB_SLASH)
			{
				if(faceRight)
				{
					sprInfo.threatBox.top  = long(sprInfo.POS.y - 192);
					sprInfo.threatBox.left = long(sprInfo.POS.x + 145);
					sprInfo.threatBox.right = sprInfo.threatBox.left + 56;
					sprInfo.threatBox.bottom  = sprInfo.threatBox.top - 21;
				}
				else
				{
					sprInfo.threatBox.top  = long(sprInfo.POS.y - 192);
					sprInfo.threatBox.left = long(sprInfo.POS.x + 55);
					sprInfo.threatBox.right = sprInfo.threatBox.left + 56;
					sprInfo.threatBox.bottom  = sprInfo.threatBox.top - 21;
				}
			}
			break;
	}
}

void Boss::stun()
{
	//min + rand() % max - min + 1
	stunTime = 200 + rand() % 301;
	stunStart = clock();
	state = SB_DIE;
	this->setVel(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

void Boss::stun(int num)
{
	//min + rand() % max - min + 1
	stunTime = (200 + rand() % 301) + num;
	stunStart = clock();
	state = SB_DIE;
	this->setVel(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

void Boss::die()
{
//note the alive variable doesn't get set until animation is over, then enemy gets deleted in EntMgr->update()
	state = SB_DIE;
	anim = 0;
	aniFStart = clock();
	this->setVel(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

int Boss::getDistance(Boss *b,Player *p)
{
	D3DXVECTOR2 bPos,pPos;
	bPos.x = float(b->getDrawInfo().hitBox.left);
	bPos.y = float(b->getDrawInfo().hitBox.top);
	pPos.x = float(p->getDrawInfo().hitBox.left);
	pPos.y = float(p->getDrawInfo().hitBox.top);

	double distance = sqrt(pow((bPos.x - pPos.x),2)+pow((bPos.y - pPos.y),2));
	return int(distance);
}
