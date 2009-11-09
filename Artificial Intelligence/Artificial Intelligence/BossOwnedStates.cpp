#include <iostream>
#include "EnemyOwnedStates.h"
#include "Status.h"
#include "Enemy.h"


//-------------------------methods for Patrol-------------------------------//
Patrol* Patrol::Instance()
{
  static Patrol instance;

  return &instance;
}

void Patrol::Enter(Enemy *boss)
{
	printf("Patrolling the area...\n");
}

void Patrol::Execute(Enemy *boss)
{
	if(boss->getStatus() == OutOfRange)
		boss->ChangeState(Chase::Instance());
	if(boss->getStatus() == InRange)
	{
		printf("InRange to Attack!\n");
		boss->ChangeState(Attack::Instance());
	}
}

void Patrol::Exit(Enemy *boss)
{
	printf("Leaving Patrol State\n");
}

//-------------------------methods for Chase-------------------------------//
Chase* Chase::Instance()
{
  static Chase instance;

  return &instance;
}

void Chase::Enter(Enemy *boss)
{
	printf("Chasing after player...\n");
}

void Chase::Execute(Enemy *boss)
{
	if(boss->getStatus() == EnemyDead)
	{
		printf("Player is dead...now Patroling....\n");
		boss->ChangeState(Patrol::Instance());
	}
	if(boss->getStatus() == InRange)
	{
		printf("Going for attack!\n");
		boss->ChangeState(Attack::Instance());
	}
	printf("Chasing Player still!\n");
}


void Chase::Exit(Enemy *boss)
{
	printf("Leaving Chase State\n");
}

//-------------------------methods for Attack-------------------------------//
Attack* Attack::Instance()
{
  static Attack instance;

  return &instance;
}

void Attack::Enter(Enemy *boss)
{
	printf("Attacking player...\n");
}

void Attack::Execute(Enemy *boss)
{
	if(boss->getStatus() == EnemyDead)
	{
		printf("Enemy died from attack, returning to patrol....\n");
		boss->ChangeState(Patrol::Instance());
	}

	if(boss->getStatus() == WeakerThanEnemy)
	{
		printf("WeakerThanPlayer, now Running from player....\n");
		boss->ChangeState(RunAway::Instance());
	}
	printf("Attacking Player still!\n");
}

void Attack::Exit(Enemy *boss)
{
	printf("Leaving Attack State\n");
}

//-------------------------methods for RunAway-------------------------------//
RunAway* RunAway::Instance()
{
  static RunAway instance;

  return &instance;
}

void RunAway::Enter(Enemy *boss)
{
	printf("Running from player...\n");
}

void RunAway::Execute(Enemy *boss)
{
	if(boss->getStatus() == OutOfRange)
	{
		printf("Enemy out of Range...Back to Chasing!\n");
		boss->ChangeState(Chase::Instance());
	}
	else if(boss->getStatus() == EnemyDead)
	{
		printf("While Running Enemy died..Back to Patrolling\n");
		boss->ChangeState(Patrol::Instance());
	}
	printf("Running away from player still!\n");
}

void RunAway::Exit(Enemy *boss)
{
	printf("Leaving RunAway State\n");
}