#include "Boss.h"
#include "Enemy.h"
#include "EnemyOwnedStates.h"
#include "BossOwnedStates.h"

Boss::Boss(int ID):Enemy(ID)
{}

//possible bug: idk if passing a char array will get the c-str that its supposed to
Boss::Boss(int ID, char KEY, D3DXVECTOR3 pos, spriteSheet *ptr)
			:Enemy(ID, KEY, pos, ptr)
{}

bool Boss::isAlive()
{
	return true;
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

void Boss::UpdateState(Player *p)
{
	CurrentState->Execute(this,p);

	//temp code: IM USING STATE/ANIM FROM BGE FOR RIGHT NOW.
	clock_t now = clock();
	switch(state)
	{
		case SB_IDLE:
			if(now - aniFStart >= SBANIMATION)
			{
				if(anim < SB_IDLE_FRAME-1)
					anim++;
				else
					anim = 0;

				aniFStart = now;
			}
			break;
		
	};

	calcDrawRECT();
}
/*
void Boss::ChangeState(State<Boss>* pNewState)
{
	CurrentState->Exit(this);
	CurrentState = pNewState;
	CurrentState->Enter(this);
}*/
