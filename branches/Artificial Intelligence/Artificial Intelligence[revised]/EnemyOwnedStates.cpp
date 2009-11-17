#include <iostream>
#include "EnemyOwnedStates.h"
#include "Status.h"
#include "Enemy.h"

//-------------------------methods for Idle-------------------------------//
Idle* Idle::Instance()
{
  static Idle instance;

  return &instance;
}

void Idle::Enter(Enemy *enemy)
{
	printf("Idling the area...\n");
}

void Idle::Execute(Enemy* enemy)
{
	printf("Idling...\n");

	if(enemy->getStatus() == InFace)
	{
		printf("Being threatened...changing state");
		enemy->ChangeState(RunAway::Instance());
	}
}

void Idle::Exit(Enemy *enemy)
{
	printf("Leaving Idle State\n");
}

//-------------------------methods for Patrol-------------------------------//
//template<class entity_type>
Patrol* Patrol::Instance()
{
  static Patrol instance;

  return &instance;
}

//template<entity_type>
void Patrol::Enter(Enemy *enemy)
{
	printf("Patrolling the area...\n");
}

//template<entity_type>
void Patrol::Execute(Enemy *enemy)
{
	if(enemy->getStatus() == OutOfRange)
	{
		printf("Enemy outside of Range");
		enemy->ChangeState(Chase::Instance());
	}
	if(enemy->getStatus() == InRange)
	{
		printf("InRange to Attack!\n");
		enemy->ChangeState(Attack::Instance());
	}
}

//template<entity_type>
void Patrol::Exit(Enemy *enemy)
{
	printf("Leaving Patrol State\n");
}

//-------------------------methods for Chase-------------------------------//
Chase* Chase::Instance()
{
  static Chase instance;

  return &instance;
}

void Chase::Enter(Enemy *enemy)
{
	printf("Chasing after player...\n");
}

void Chase::Execute(Enemy *enemy)
{
	if(enemy->getStatus() == EnemyDead)
	{
		printf("Player is dead...now Patroling....\n");
		enemy->ChangeState(Patrol::Instance());
	}
	if(enemy->getStatus() == InRange)
	{
		printf("Going for attack!\n");
		enemy->ChangeState(Attack::Instance());
	}
	printf("Chasing Player still!\n");
}


void Chase::Exit(Enemy *enemy)
{
	printf("Leaving Chase State\n");
}

//-------------------------methods for Attack-------------------------------//
Attack* Attack::Instance()
{
  static Attack instance;

  return &instance;
}

void Attack::Enter(Enemy *enemy)
{
	printf("Attacking player...\n");
}

void Attack::Execute(Enemy *enemy)
{
	if(enemy->getStatus() == EnemyDead)
	{
		printf("Enemy died from attack, returning to patrol....\n");
		enemy->ChangeState(Patrol::Instance());
	}

	if(enemy->getStatus() == WeakerThanEnemy)
	{
		printf("WeakerThanPlayer, now Running from player....\n");
		enemy->ChangeState(RunAway::Instance());
	}
	printf("Attacking Player still!\n");
}

void Attack::Exit(Enemy *enemy)
{
	printf("Leaving Attack State\n");
}

//-------------------------methods for RunAway-------------------------------//
RunAway* RunAway::Instance()
{
  static RunAway instance;

  return &instance;
}

void RunAway::Enter(Enemy *enemy)
{
	printf("Running from player...\n");
}

void RunAway::Execute(Enemy *enemy)
{
	if(enemy->getStatus() == OutOfRange)
	{
		printf("Enemy out of Range...Back to Chasing!\n");
		enemy->ChangeState(Chase::Instance());
	}
	else if(enemy->getStatus() == EnemyDead)
	{
		printf("While Running Enemy died..Back to Patrolling\n");
		enemy->ChangeState(Patrol::Instance());
	}
	printf("Running away from player still!\n");
}

void RunAway::Exit(Enemy *enemy)
{
	printf("Leaving RunAway State\n");
}