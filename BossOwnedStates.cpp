#include <iostream>
#include "BossOwnedStates.h"
#include "EnemyOwnedStates.h"
#include "Status.h"
#include "Enemy.h"



//-------------------------methods for LessThanFifty-------------------------------//
LessThanFifty* LessThanFifty::Instance()
{
  static LessThanFifty instance;

  return &instance;
}

void LessThanFifty::Enter(Enemy *boss)
{
	printf("<50% Health...\n");
}

void LessThanFifty::Execute(Enemy *boss)
{
	if(boss->getStatus() == EnemyDead)
	{
		printf("Player is dead...now Idleing....\n");
		boss->ChangeState(Idle::Instance());
	}
	if(boss->getStatus() == InRange)
	{
		printf("Going for attack!\n");
		boss->ChangeState(LessThanTwentyFive::Instance());
	}
	printf("I am currently <50% Health!\n");
}


void LessThanFifty::Exit(Enemy *boss)
{
	printf("Leaving LessThanFifty State\n");
}

//-------------------------methods for LessThanTwentyFive-------------------------------//
LessThanTwentyFive* LessThanTwentyFive::Instance()
{
  static LessThanTwentyFive instance;

  return &instance;
}

void LessThanTwentyFive::Enter(Enemy *boss)
{
	printf("<25% Health...\n");
}

void LessThanTwentyFive::Execute(Enemy *boss)
{
	if(boss->getStatus() == EnemyDead)
	{
		printf("Enemy died from attack, returning to patrol....\n");
		boss->ChangeState(Idle::Instance());
	}

	if(boss->getStatus() == WeakerThanEnemy)
	{
		printf("WeakerThanPlayer, going crazy!!!!\n");
	}
	printf("I am currently <25% Health!\n");
}

void LessThanTwentyFive::Exit(Enemy *boss)
{
	printf("Leaving LessThanTwentyFive State\n");
}

