#include "Boss.h"
#include "BossOwnedStates.h"

Boss::Boss(int ID):BaseGameEntity(ID),
				   status(InRange),
				   CurrentState(Patrol::Instance())
{}

bool Boss::isAlive()
{
	if(health > 0 && health <= 200)
		return true;
	else
		return false;
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

void Boss::UpdateState()
{
	CurrentState->Execute(this);
}

void Boss::ChangeState(State* pNewState)
{
	CurrentState->Exit(this);
	CurrentState = pNewState;
	CurrentState->Enter(this);
}