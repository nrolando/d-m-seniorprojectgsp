#include "Boss.h"
#include "Enemy.h"
#include "EnemyOwnedStates.h"
#include "BossOwnedStates.h"

Boss::Boss(int ID):Enemy(ID)
{}

//possible bug: idk if passing a char array will get the c-str that its supposed to
Boss::Boss(int ID, char KEY, D3DXVECTOR3 pos)
			:Enemy(ID, KEY, pos)
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
/*
void Boss::UpdateState()
{
	CurrentState->Execute(this);
}

void Boss::ChangeState(State<Boss>* pNewState)
{
	CurrentState->Exit(this);
	CurrentState = pNewState;
	CurrentState->Enter(this);
}*/