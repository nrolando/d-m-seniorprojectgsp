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
	if(boss->getDistance(boss,player) <= SB_SIGHT_RANGE && boss->getCurrHealth() >= 80)
	{
		boss->ChangeState(Aggressive::Instance());
		printf("Player in Range Charge Him\n");
	}
	else if(boss->getDistance(boss,player) <= SB_SIGHT_RANGE && boss->getCurrHealth() >= 50 &&
	boss->getDistance(boss,player) <= SB_SIGHT_RANGE && boss->getCurrHealth() <= 80)
	{
		boss->ChangeState(Defensive::Instance());
		printf("Boss: Less than 80percent switching to defensive mode");
	}
	else if(boss->getDistance(boss,player) <= SB_SIGHT_RANGE && boss->getCurrHealth() >= 0 &&
	boss->getDistance(boss,player) <= SB_SIGHT_RANGE && boss->getCurrHealth() <= 50)
	{
		boss->ChangeState(Beserk::Instance());
		printf("Boss: Less than 50percent GOIN INSANE!!!");
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
		boss->ChangeState(Attacking::Instance());
	//Rush at the Player if within Range
	else if(boss->getDistance(boss,player) >= SB_RUSH_RANGE && boss->getDistance(boss,player) > SB_ATTACK_RANGE) 
	{
		boss->UpdateStat(2,SB_RUN_SPEED);
		boss->ChangeState(Rush::Instance());
	}
	else
	{
		boss->movement('n');
		boss->setStatus(SB_IDLE);
	}
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
	printf("Entering Defensive state\n");
}

void Defensive::Execute(Boss* boss, Player* player,std::vector<BaseGameEntity*> EntMgr)
{
	if(player->getStatus() == KICK || player->getStatus() == PUNCH || player->getStatus() == COMBO1)
		boss->ChangeState(Avoid::Instance());
	if(boss->getDistance(boss,player) <= SB_RUSH_RANGE &&
		boss->getDistance(boss,player) > SB_ATTACK_RANGE)
	{
		if(boss->isFacing() && player->isFacing() ||
		!boss->isFacing() && !player->isFacing())
		{
			boss->UpdateStat(2,SB_RUN_SPEED);
			boss->ChangeState(Rush::Instance());
		}
	}
	else if(boss->getDistance(boss,player) <= SB_ATTACK_RANGE)
		boss->ChangeState(Attacking::Instance());
	else
		boss->ChangeState(Avoid::Instance());
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
	printf("<25percent Health...\n");
}

void Beserk::Execute(Boss* boss, Player* player,std::vector<BaseGameEntity*> EntMgr)
{
	//if(!boss->isAnimFinished() && !taunted)
	//	boss->setStatus(SB_TAUNT);
	//else if(boss->getStatus() == SB_TAUNT && boss->getAnimFrame())
	//	taunted = true;
		
	if(boss->getDistance(boss,player) <= SB_ATTACK_RANGE)
		boss->ChangeState(Attacking::Instance());
	if(player->getStatus() == KICK || player->getStatus() == PUNCH || player->getStatus() == COMBO1)
		boss->ChangeState(Avoid::Instance());
	if(boss->getDistance(boss,player) <= SB_RUSH_RANGE &&
		boss->getDistance(boss,player) > SB_ATTACK_RANGE)
	{
		if(boss->isFacing() && player->isFacing() ||
		!boss->isFacing() && !player->isFacing())
		{
			boss->UpdateStat(2,SB_RUN_SPEED);
			boss->ChangeState(Rush::Instance());
		}
	}
	else if(boss->getDistance(boss,player) <= SB_ATTACK_RANGE)
		boss->ChangeState(Attacking::Instance());
	else
	{
		boss->ChangeState(Avoid::Instance());
	}
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
	boss->setStatus(SB_IDLE);
	boss->movement('n');
	boss->UpdateStat(2,SB_RUN_SPEED*1.4);
	if(boss->getDistance(boss,player) < SB_ATTACK_RANGE-40.0f)
	{
		boss->UpdateStat(2,SB_WALK_SPEED);
		boss->ChangeState(Attacking::Instance());
	}
	if(player->getDrawInfo().hitBox.top <= boss->getDrawInfo().hitBox.top-YRANGE_OFFSET &&
	boss->getDistance(boss,player) >= SB_ATTACK_RANGE)
	{
		boss->setStatus(SB_RUN);
		boss->movement('d');
	}
	if(player->getDrawInfo().hitBox.bottom >= boss->getDrawInfo().hitBox.bottom+YRANGE_OFFSET &&
	boss->getDistance(boss,player) >= SB_ATTACK_RANGE)
	{
		boss->setStatus(SB_RUN);
		boss->movement('u');
	}
	if(player->getDrawInfo().hitBox.right-XRANGE_OFFSET <= boss->getDrawInfo().hitBox.left &&
	boss->getDistance(boss,player) >= SB_ATTACK_RANGE)
	{
		boss->setStatus(SB_RUN);
		boss->movement('l');
	}
	if(player->getDrawInfo().hitBox.left+XRANGE_OFFSET >= boss->getDrawInfo().hitBox.right &&
	boss->getDistance(boss,player) >= SB_ATTACK_RANGE)
	{
		boss->setStatus(SB_RUN);
		boss->movement('r');
	}
	if(boss->getDistance(boss,player) <= SB_ATTACK_RANGE)
	{
		if(boss->Missed() && boss->getLastAttFrame() == -1)
			++attacks;
		if(attacks >= 1)
		{
			attacks = 0;
			boss->ChangeState(Stationary::Instance());
			boss->UpdateStat(2,SB_WALK_SPEED);
		}
		boss->setStatus(SB_SLASH);
	}
	if(boss->getDistance(boss,player) >= SB_RUSH_RANGE)
		boss->ChangeState(Stationary::Instance());
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
	if(boss->getDistance(boss,player) >= SB_ATTACK_RANGE-XRANGE_OFFSET)
		boss->ChangeState(Stationary::Instance());
	if(player->getDrawInfo().hitBox.top <= boss->getDrawInfo().hitBox.top-YRANGE_OFFSET &&
	boss->getDistance(boss,player) >= SB_ATTACK_RANGE)
	{
		boss->setStatus(SB_WALK);
		boss->movement('d');
	}
	if(player->getDrawInfo().hitBox.bottom >= boss->getDrawInfo().hitBox.bottom+YRANGE_OFFSET)
	{
		boss->setStatus(SB_WALK);
		boss->movement('u');
	}
	if(player->getDrawInfo().hitBox.right-XRANGE_OFFSET <= boss->getDrawInfo().hitBox.left)
	{
		boss->setStatus(SB_WALK);
		boss->movement('l');
	}
	if(player->getDrawInfo().hitBox.left+XRANGE_OFFSET >= boss->getDrawInfo().hitBox.right)
	{
		boss->setStatus(SB_WALK);
		boss->movement('r');
	}
	if(boss->getDistance(boss,player) <= SB_ATTACK_RANGE-40.0f)
	{
		if(boss->Missed() && boss->getLastAttFrame() == -1)
			++attacks;
		if(attacks >= 1)
		{
			attacks = 0;
			boss->ChangeState(Stationary::Instance());
			boss->UpdateStat(2,SB_WALK_SPEED);
		}
		boss->setStatus(SB_KICK);
		boss->movement('n');
	}
}

void Attacking::Exit(Boss *boss)
{
	printf("Leaving Attacking State\n");
}

//-------------------------methods for Avoid-------------------------------//
Avoid* Avoid::Instance()
{
  static Avoid instance;

  return &instance;
}

void Avoid::Enter(Boss *boss)
{
	printf("Boss: Avoiding...\n");
}

void Avoid::Execute(Boss* boss, Player* player, std::vector<BaseGameEntity*> EntMgr)
{
	boss->setStatus(SB_IDLE);
	boss->movement('n');
	boss->UpdateStat(2,SB_BACKUP_SPEED);
	if(boss->getDistance(boss,player) <= SB_DEFENSE_RANGE)
	{
		if(boss->getDistance(boss,player) >= SB_ATTACK_RANGE-XRANGE_OFFSET)
			boss->ChangeState(Stationary::Instance());
		if(player->getDrawInfo().hitBox.top <= boss->getDrawInfo().hitBox.top-YRANGE_OFFSET)
		{
			boss->setStatus(SB_RUN);
			boss->movement('u');
		}
		if(player->getDrawInfo().hitBox.bottom >= boss->getDrawInfo().hitBox.bottom+YRANGE_OFFSET)
		{
			boss->setStatus(SB_RUN);
			boss->movement('d');
		}
		if(player->getDrawInfo().hitBox.right-XRANGE_OFFSET <= boss->getDrawInfo().hitBox.left)
		{
			boss->setStatus(SB_RUN);
			boss->movement('r');
		}
		if(player->getDrawInfo().hitBox.left+XRANGE_OFFSET >= boss->getDrawInfo().hitBox.right)
		{
			boss->setStatus(SB_RUN);
			boss->movement('l');
		}
	}
	if(boss->getDistance(boss,player) >= SB_DEFENSE_RANGE)
	{
		boss->UpdateStat(2,SB_WALK_SPEED);
		boss->ChangeState(Stationary::Instance());
	}
}

void Avoid::Exit(Boss *boss)
{
	printf("Leaving Avoid State\n");
}