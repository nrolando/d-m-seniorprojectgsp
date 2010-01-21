#include <iostream>
#include "BossOwnedStates.h"
#include "EnemyOwnedStates.h"
#include "Boss.h"
#include "Status.h"


//-------------------------methods for LessThanFifty-------------------------------//
LessThanFifty* LessThanFifty::Instance()
{
  static LessThanFifty instance;

  return &instance;
}

void LessThanFifty::Enter(Boss *boss)
{
	printf("<50% Health...\n");
}

void LessThanFifty::Execute(Boss* boss, D3DXVECTOR3 playerPos)
{
	if(boss->getStatus() == EnemyDead)
	{
		printf("Player is dead...now Idleing....\n");
		//boss->ChangeState(Idle::Instance());
	}
	if(boss->getStatus() == InRange)
	{
		printf("Going for attack!\n");
		//boss->ChangeState(LessThanTwentyFive::Instance());
	}
	printf("I am currently <50% Health!\n");
}


void LessThanFifty::Exit(Boss *boss)
{
	printf("Leaving LessThanFifty State\n");
}

//-------------------------methods for LessThanTwentyFive-------------------------------//
LessThanTwentyFive* LessThanTwentyFive::Instance()
{
  static LessThanTwentyFive instance;

  return &instance;
}

void LessThanTwentyFive::Enter(Boss *boss)
{
	printf("<25% Health...\n");
}

void LessThanTwentyFive::Execute(Boss* boss, D3DXVECTOR3 playerPos)
{
	if(boss->getStatus() == EnemyDead)
	{
		printf("Enemy died from attack, returning to patrol....\n");
		//boss->ChangeState(Idle::Instance());
	}

	if(boss->getStatus() == WeakerThanEnemy)
	{
		printf("WeakerThanPlayer, going crazy!!!!\n");
	}
	printf("I am currently <25% Health!\n");
}

void LessThanTwentyFive::Exit(Boss *boss)
{
	printf("Leaving LessThanTwentyFive State\n");
}

