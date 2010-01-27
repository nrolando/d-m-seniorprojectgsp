#include <iostream>
#include "BossOwnedStates.h"
#include "Player.h"
#include "Boss.h"
#include "Status.h"


//-------------------------methods for Aggressive-------------------------------//
Aggressive* Aggressive::Instance()
{
  static Aggressive instance;

  return &instance;
}

void Aggressive::Enter(Boss *boss)
{
	printf("Commencing Battle\n");
}

void Aggressive::Execute(Boss* boss, Player* player)
{
	if(boss->getDistance(boss->getPos(),player->getPos()) < SB_ATTACK_RANGE)
	{
		boss->setState(SB_RUN);

		if(boss->getPos().y > player->getPos().y+SB_RANGE_OFFSET)
		{
			boss->setState(SB_WALK);
			boss->movement('d');
		}
		if(boss->getPos().y < player->getPos().y-SB_RANGE_OFFSET) 
		{
			boss->setState(SB_WALK);
			boss->movement('u');
		}
		if(boss->getPos().x > player->getPos().x+SB_RANGE_OFFSET)
		{
			boss->setState(SB_WALK);
			boss->movement('l');
		}
		if(boss->getPos().x < player->getPos().x+SB_RANGE_OFFSET) 
		{
			boss->setState(SB_WALK);
			boss->movement('r');
		}
	}
	printf("I am currently <50% Health!\n");
}


void Aggressive::Exit(Boss *boss)
{
	printf("Leaving Defensive State\n");
}


//-------------------------methods for Defensive-------------------------------//
Defensive* Defensive::Instance()
{
  static Defensive instance;

  return &instance;
}

void Defensive::Enter(Boss *boss)
{
	printf("<50% Health...\n");
}

void Defensive::Execute(Boss* boss, Player* player)
{
	if(boss->getStatus() == EnemyDead)
	{
		printf("Player is dead...now Idleing....\n");
		//boss->ChangeState(Idle::Instance());
	}
	if(boss->getStatus() == InRange)
	{
		printf("Going for attack!\n");
		//boss->ChangeState(Beserk::Instance());
	}
	printf("I am currently <50% Health!\n");
}


void Defensive::Exit(Boss *boss)
{
	printf("Leaving Defensive State\n");
}

//-------------------------methods for Beserk-------------------------------//
Beserk* Beserk::Instance()
{
  static Beserk instance;

  return &instance;
}

void Beserk::Enter(Boss *boss)
{
	printf("<25% Health...\n");
}

void Beserk::Execute(Boss* boss, Player* player)
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

void Beserk::Exit(Boss *boss)
{
	printf("Leaving Beserk State\n");
}

