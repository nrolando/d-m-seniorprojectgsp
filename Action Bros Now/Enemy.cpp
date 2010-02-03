#include <cassert>
#include <iostream>
#include "EnemyOwnedStates.h"
#include "Player.h" 
#include "Enemy.h"

Enemy::Enemy(int ID):BaseGameEntity(ID),
					 status(InRange),
					 CurrentState(Idle::Instance())
{
	faceRight = false;
	miss = false;
}

//possible bug: idk if passing a char array will get the c-str that its supposed to
Enemy::Enemy(int ID, char KEY, D3DXVECTOR3 pos, spriteSheet *ptr) 
:BaseGameEntity(ID, KEY, pos, ptr), status(InRange), CurrentState(Idle::Instance())
{
	miss = false;
	if(KEY == SOLDIER1)
		faceRight = false;
	else
		faceRight = false;
}

void Enemy::UpdateState(Player *p,std::vector<BaseGameEntity*> e)
{
	if(state != E_STUN && state != E_FALL)
		CurrentState->Execute(this,p,e);

	//temp code: IM USING STATE/ANIM FROM BGE FOR RIGHT NOW.
	clock_t now = clock();
	switch(state)
	{
	case E_IDLE:
		if(now - aniFStart >= IDLEANIMATION)
		{
			if(anim < IDLEFRAME-1)
			{
				anim++;
			}
			else
			{
				anim = 0;
			}

			aniFStart = now;
		}
		break;
	case E_WALK:
		if(now - aniFStart >= WALKFRAMETIME)
		{
			if(anim < CSWALKFRAME-1)
			{
				anim++;
			}
			else
			{
				anim = 0;
			}

			aniFStart = now;
		}
		break;
	case E_ATTACK1:
		//Makes sure to face the character correctly when attacking
		if(p->getPos().x < this->getPos().x && isRotated())
			rotate();
		else if(p->getPos().x > this->getPos().x && !isRotated())
			rotate();

		setHitFrames(2, -1, -1);
		setPower(5);
		if(now - aniFStart >= ANIMATIONGAP)
		{
			if(anim < ATTACKFRAME-1)
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
	case E_FALL:
		if(now - aniFStart >= DEATHFRAMETIME)
		{
			if(health > 0)
			{
				if(anim < FALLFRAME-1)
					anim++;
				else
					anim = 0;

			}
			else
			{
				if(anim < FALLFRAME-2)
					anim++;
				else
					alive = false;
			}
			aniFStart = now;
		}
		break;
	case E_STUN:
		//if has been long enough
		if(now - stunStart >= stunTime)
		{
			//switch to first frame of idle state
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

void Enemy::ChangeState(State<Enemy, Player>* pNewState)
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
			if(health > 0)
				health += val;
			else
			{
				health = 0;
				alive = false;
			}
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
	case SOLDIER_BOSS:
		if(faceRight)
			state_frame = state;
		else
			state_frame = state + SBTOTALSTATES;
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
		sprInfo.hitBox.top  = long(sprInfo.POS.y - 170);
		sprInfo.hitBox.left = long(sprInfo.POS.x + 102);
		sprInfo.hitBox.right = sprInfo.hitBox.left + 48;
		sprInfo.hitBox.bottom  = sprInfo.hitBox.top - 86;
		break;
	case SOLDIER_BOSS:
		sprInfo.hitBox.top  = long(sprInfo.POS.y - 181);
		sprInfo.hitBox.left = long(sprInfo.POS.x + 101);
		sprInfo.hitBox.right = sprInfo.hitBox.left + 45;
		sprInfo.hitBox.bottom  = sprInfo.hitBox.top - 75;
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
		else
		{	//make threatbox off world
			sprInfo.threatBox.top  = -8880;
			sprInfo.threatBox.left = -8880;
			sprInfo.threatBox.right = -8880;
			sprInfo.threatBox.bottom = -8880;
		}
		break;
	case SOLDIER_BOSS:
		if(state == E_ATTACK1)
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
		else
		{	//make threatbox off world
			sprInfo.threatBox.top  = -8880;
			sprInfo.threatBox.left = -8880;
			sprInfo.threatBox.right = -8880;
			sprInfo.threatBox.bottom = -8880;
		}
	default:
		break;
	};
}

bool Enemy::MovementPossible(std::vector<BaseGameEntity*> EMgr)
{
	for(unsigned int i=0;i<EMgr.size();++i)
	{
		if(this != EMgr[i])
		{
			if (getDrawInfo().hitBox.top >= EMgr[i]->getDrawInfo().hitBox.bottom &&
			getDrawInfo().hitBox.top <= EMgr[i]->getDrawInfo().hitBox.top)
			{return false;}
			if (getDrawInfo().hitBox.left >= EMgr[i]->getDrawInfo().hitBox.left &&
			getDrawInfo().hitBox.left <= EMgr[i]->getDrawInfo().hitBox.right)
			{return false;}
			if (getDrawInfo().hitBox.right >= EMgr[i]->getDrawInfo().hitBox.left &&
			getDrawInfo().hitBox.right <= EMgr[i]->getDrawInfo().hitBox.right)
			{return false;}
			if (getDrawInfo().hitBox.bottom <= EMgr[i]->getDrawInfo().hitBox.top &&
			getDrawInfo().hitBox.bottom >= EMgr[i]->getDrawInfo().hitBox.bottom)
			{return false;}
		}
	}
	return true;
}
void Enemy::AvoidEntity(Player* p,std::vector<BaseGameEntity*> EMgr)
{
	//for(unsigned int i= 0;i<EMgr.size();++i)
	//{
	//	//If the entity being checked is not itself, within sight range, and has not been tagged to stand still
	//	if(this != EMgr[i] && getDistance(this,EMgr[i]->getPos()) < AVOID_RANGE /*&& !EMgr[i]->isTagged()*/)
	//	{
	//		//EMgr[i]->tag();
	//		if(getPos().x+getDrawInfo().hitBox.right >= EMgr[i]->getPos().x){
	//			vel.y = speed; 
	//			vel.x = -speed;
	//			vel.z = 0;}
	//		if(getPos().x >= EMgr[i]->getPos().x+EMgr[i]->getDrawInfo().hitBox.right){
	//			vel.y = -speed; 
	//			vel.x = speed; 
	//			vel.z = 0;}
	//		if(getPos().y-getDrawInfo().hitBox.bottom <= EMgr[i]->getPos().y){
	//			faceRight = true;
	//			vel.x = speed; 
	//			vel.y = -speed;
	//			vel.z = 0;}
	//		if(getPos().y >= EMgr[i]->getPos().y-EMgr[i]->getDrawInfo().hitBox.bottom){
	//			faceRight = false;
	//			vel.x = -speed;
	//			vel.y = speed;
	//			vel.z = 0;}
	//	}
		/*else if(getDistance(getPos(),EMgr[i]->getPos()) >= AVOID_RANGE/5 && EMgr[i]->isTagged() ||
		!isAlive())
		{EMgr[i]->untag();}*/
	/*}*/
}
void Enemy::movement(char dir,Player* p,std::vector<BaseGameEntity*> EMgr)
{
	clock_t now = clock();
	switch(dir)
	{
		case 'l':
			faceRight = false;
			vel.x = -speed;
			vel.y = vel.z = 0;

			if(!MovementPossible(EMgr))
				AvoidEntity(p,EMgr);
			break;
		case 'd':
			vel.y = -speed;
			vel.x = vel.z = 0;

			if(!MovementPossible(EMgr))
				AvoidEntity(p,EMgr);
			break;
		case 'r':
			faceRight = true;
			vel.x = speed;
			vel.y = vel.z = 0;

			if(!MovementPossible(EMgr))
				AvoidEntity(p,EMgr);
			break;
		case 'u':
			vel.y = speed; 
			vel.x = vel.z = 0;

			if(!MovementPossible(EMgr))
				AvoidEntity(p,EMgr);
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
	anim = 0;
	aniFStart = clock();
	this->setVel(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

int Enemy::getDistance(Enemy* e,Player* p)
{
	D3DXVECTOR3 ePos,pPos;
	ePos.x = float(e->getDrawInfo().hitBox.left);
	ePos.y = float(e->getDrawInfo().hitBox.top);
	pPos.x = float(p->getDrawInfo().hitBox.left);
	pPos.y = float(p->getDrawInfo().hitBox.top);

	double distance = sqrt(pow((ePos.x - pPos.x),2)+pow((ePos.y - pPos.y),2));
	return int(distance);
}
void Enemy::die()
{
//note the alive variable doesn't get set until animation is over, then enemy gets deleted in EntMgr->update()
	state = E_FALL;
	anim = 0;
	aniFStart = clock();
	this->setVel(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}