#include <iostream>
#include "BossOwnedStates.h"
#include "EnemyOwnedStates.h"
#include "Status.h"
#include "Enemy.h"


//-------------------------methods for Idle-------------------------------//
Idle* Idle::Instance()
{
  static Idle instance;

  return &instance;
}

void Idle::Enter(Enemy *boss)
{
	printf("Idling the area...\n");
}

void Idle::Execute(Enemy* boss)
{
	printf("Idling...\n");
	boss->ChangeState(Patrol::Instance());
}

void Idle::Exit(Enemy *boss)
{
	printf("Leaving Idle State\n");
}

////-------------------------methods for LessThanFifty-------------------------------//
//LessThanFifty* LessThanFifty::Instance()
//{
//  static LessThanFifty instance;
//
//  return &instance;
//}
//
//void LessThanFifty::Enter(Boss *boss)
//{
//	printf("<50% Health...\n");
//}
//
//void LessThanFifty::Execute(Boss *boss)
//{
//	if(boss->getStatus() == EnemyDead)
//	{
//		printf("Player is dead...now Idleing....\n");
//		boss->ChangeState(Idle::Instance());
//	}
//	if(boss->getStatus() == InRange)
//	{
//		printf("Going for attack!\n");
//		boss->ChangeState(LessThanTwentyFive::Instance());
//	}
//	printf("I am currently <50% Health!\n");
//}
//
//
//void LessThanFifty::Exit(Boss *boss)
//{
//	printf("Leaving LessThanFifty State\n");
//}
//
////-------------------------methods for LessThanTwentyFive-------------------------------//
//LessThanTwentyFive* LessThanTwentyFive::Instance()
//{
//  static LessThanTwentyFive instance;
//
//  return &instance;
//}
//
//void LessThanTwentyFive::Enter(Boss *boss)
//{
//	printf("<25% Health...\n");
//}
//
//void LessThanTwentyFive::Execute(Boss *boss)
//{
//	if(boss->getStatus() == EnemyDead)
//	{
//		printf("Enemy died from attack, returning to patrol....\n");
//		boss->ChangeState(Idle::Instance());
//	}
//
//	if(boss->getStatus() == WeakerThanEnemy)
//	{
//		printf("WeakerThanPlayer, now Running from player....\n");
//		boss->ChangeState(RunAway::Instance());
//	}
//	printf("I am currently <25% Health!\n");
//}
//
//void LessThanTwentyFive::Exit(Boss *boss)
//{
//	printf("Leaving LessThanTwentyFive State\n");
//}
//
