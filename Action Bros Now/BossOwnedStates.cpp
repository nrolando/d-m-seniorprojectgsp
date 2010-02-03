#include <iostream>
#include "BossOwnedStates.h"
#include "Player.h"
#include "Boss.h"
#include "Status.h"


//-------------------------methods for Stationary-------------------------------//
Stationary* Stationary::Instance()
{
  static Stationary instance;

  return &instance;
}

void Stationary::Enter(Boss *boss)
{
	printf("Boss: Idling...\n");
}

void Stationary::Execute(Boss* boss, Player* player, std::vector<BaseGameEntity*> EntMgr)
{
	if(boss->getDistance(boss,player) <= SB_RUSH_RANGE)
	{
		boss->ChangeState(Aggressive::Instance());
		printf("Player in Range Charge Him\n");
	}
}


void Stationary::Exit(Boss *boss)
{
	printf("Leaving Stationary State\n");
}

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

void Aggressive::Execute(Boss* boss, Player* player,std::vector<BaseGameEntity*> EntMgr)
{
	if(boss->getDistance(boss,player) <= SB_ATTACK_RANGE)
	{
		boss->ChangeState(Attacking::Instance());
		boss->movement('n');
	}
	//Rush at the Player if within Range
	else if(boss->getDistance(boss,player) <= SB_RUSH_RANGE &&
	boss->getDistance(boss,player) > SB_ATTACK_RANGE) 
		boss->ChangeState(Rush::Instance());
	else
		boss->setStatus(SB_TAUNT);
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

void Defensive::Execute(Boss* boss, Player* player,std::vector<BaseGameEntity*> EntMgr)
{
	if(boss->getStatus() == EnemyDead)
	{
		printf("Player is dead...now Idleing....\n");
		//boss->ChangeState(Idle::Instance());
	}
	if(boss->getStatus() == InRange)
	{
		printf("Going for Attacking!\n");
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

void Beserk::Execute(Boss* boss, Player* player,std::vector<BaseGameEntity*> EntMgr)
{
	if(boss->getStatus() == EnemyDead)
	{
		printf("Enemy died from Attacking, returning to patrol....\n");
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

//-------------------------methods for Rush-------------------------------//
Rush* Rush::Instance()
{
  static Rush instance;

  return &instance;
}

void Rush::Enter(Boss *boss)
{
	printf("Boss: Idling...\n");
	boss->setStatus(SB_RUN);
}

void Rush::Execute(Boss* boss, Player* player, std::vector<BaseGameEntity*> EntMgr)
{
	if(boss->getDistance(boss,player) < SB_ATTACK_RANGE)
		boss->ChangeState(Attacking::Instance());
	if(boss->getDistance(boss,player) <= SB_RUSH_RANGE &&
	boss->getDistance(boss,player) > SB_ATTACK_RANGE)
	{
		if(boss->getPos().y > player->getPos().y+SB_RANGE_OFFSET)
		{
			boss->setStatus(SB_RUN);
			boss->movement('d');
		}
		//Boss position less than [since coordinates are negative, below the player]
		//the player's current position 
		if(boss->getPos().y < player->getPos().y-player->getDrawInfo().hitBox.right) 
		{
			boss->setStatus(SB_RUN);
			boss->movement('u');
		}
		if(boss->getPos().x > player->getPos().x+SB_RANGE_OFFSET)
		{
			boss->setStatus(SB_RUN);
			boss->movement('l');
		}
		if(boss->getPos().x < player->getPos().x+SB_RANGE_OFFSET) 
		{
			boss->setStatus(SB_RUN);
			boss->movement('r');
		}
		printf("Player in Range Charge Him\n");
	}
}


void Rush::Exit(Boss *boss)
{
	printf("Leaving Rush State\n");
}

//-------------------------methods for Attacking-------------------------------//
Attacking* Attacking::Instance()
{
  static Attacking instance;

  return &instance;
}

void Attacking::Enter(Boss *boss)
{
	printf("Boss: Attackinging...\n");
}

void Attacking::Execute(Boss* boss, Player* player, std::vector<BaseGameEntity*> EntMgr)
{
	boss->movement('n');
	if(boss->getStatus() == SB_RUN)
	{
		boss->setStatus(SB_SLASH);
		if(boss->isAnimFinished())
			boss->ChangeState(Aggressive::Instance());
	}
	else
	{
		boss->setStatus(SB_KICK);
		if(boss->isAnimFinished())
			boss->ChangeState(Aggressive::Instance());
	}

}

void Attacking::Exit(Boss *boss)
{
	printf("Leaving Attacking State\n");
}