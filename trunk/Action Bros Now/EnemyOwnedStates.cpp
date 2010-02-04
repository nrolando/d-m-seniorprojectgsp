#include "EnemyOwnedStates.h"
#include "Enemy.h"
#include "Player.h"
#include "Status.h"

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

void Idle::Execute(Enemy* enemy, Player* player,std::vector<BaseGameEntity*> EntMgr)
{
	printf("Idling...\n");

	if(enemy->getDistance(enemy,player) <= CHASE_RANGE)
	{
		printf("Within Range...changing state");
		enemy->ChangeState(Chase::Instance());
	}
	else if(enemy->getDistance(enemy,player) <= ATTACK_RANGE)
	{
		printf("Within Attack Range...changing state");
		enemy->ChangeState(Attack::Instance());
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
void Patrol::Execute(Enemy* enemy, Player* player,std::vector<BaseGameEntity*> EntMgr)
{
	if(enemy->getStatus() == OutOfRange)
	{
		printf("Enemy outside of Range");
		enemy->ChangeState(Chase::Instance());
	}
	if(enemy->getStatus() == InRange)
	{
		printf("InRange to Attack!\n");
		//enemy->ChangeState(Attack::Instance());
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

void Chase::Execute(Enemy* enemy, Player* player,std::vector<BaseGameEntity*> EntMgr)
{
	//set movement back to default to stop zig zagging
	enemy->movement('n',player,EntMgr);
	//If player is within attack range then switch to Attack state
	if(enemy->getDistance(enemy,player) <= ATTACK_RANGE)
			enemy->ChangeState(Attack::Instance());
	if(player->getDrawInfo().hitBox.top <= enemy->getDrawInfo().hitBox.top-YRANGE_OFFSET)
	{
		enemy->setStatus(E_WALK);
		enemy->movement('d',player,EntMgr);
	}
	if(player->getDrawInfo().hitBox.bottom >= enemy->getDrawInfo().hitBox.bottom+YRANGE_OFFSET)
	{
		enemy->setStatus(E_WALK);
		enemy->movement('u',player,EntMgr);
	}
	if(player->getDrawInfo().hitBox.right-XRANGE_OFFSET <= enemy->getDrawInfo().hitBox.left)
	{
		enemy->setStatus(E_WALK);
		enemy->movement('l',player,EntMgr);
	}
	if(player->getDrawInfo().hitBox.left+XRANGE_OFFSET >= enemy->getDrawInfo().hitBox.right)
	{
		enemy->setStatus(E_WALK);
		enemy->movement('r',player,EntMgr);
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

void Attack::Execute(Enemy* enemy, Player* player,std::vector<BaseGameEntity*> EntMgr)
{
	
	if(enemy->getDistance(enemy,player) <= ATTACK_RANGE &&
	enemy->getDistance(enemy,player) >= (ATTACK_RANGE/4))
	{	
		if(player->getStatus() == STUN && enemy->getLastAttFrame() == -1 || enemy->Missed() && enemy->getLastAttFrame() == -1)
			++attacks;
		if(attacks > 2)
		{
			attacks = 0;
			enemy->resetAtk();
			enemy->ChangeState(RunAway::Instance());	
		}
		else
		{
			enemy->movement('n',player,EntMgr);
			enemy->setStatus(E_ATTACK1);
		}
	}
	else if(float(enemy->getHealth())/float(enemy->getMaxHealth())*100 < 40.0f)
	{
		enemy->ChangeState(RunAway::Instance());
	}
	//Movement logic to get into attack range
	if(player->getDrawInfo().hitBox.top <= enemy->getDrawInfo().hitBox.top-YRANGE_OFFSET)
	{
		enemy->setStatus(E_WALK);
		enemy->movement('d',player,EntMgr);
	}
	if(player->getDrawInfo().hitBox.bottom >= enemy->getDrawInfo().hitBox.bottom+YRANGE_OFFSET)
	{
		enemy->setStatus(E_WALK);
		enemy->movement('u',player,EntMgr);
	}
	if(player->getDrawInfo().hitBox.right-XRANGE_OFFSET <= enemy->getDrawInfo().hitBox.left)
	{
		enemy->setStatus(E_WALK);
		enemy->movement('l',player,EntMgr);
	}
	if(player->getDrawInfo().hitBox.left+XRANGE_OFFSET >= enemy->getDrawInfo().hitBox.right)
	{
		enemy->setStatus(E_WALK);
		enemy->movement('r',player,EntMgr);
	}
	if(enemy->getDistance(enemy,player) > ATTACK_RANGE)
	{
		enemy->movement('n',player,EntMgr);
		enemy->ChangeState(Chase::Instance());
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

void RunAway::Execute(Enemy* enemy, Player* player,std::vector<BaseGameEntity*> EntMgr)
{
	//Zeroes out movement so that the enemy does not zig zagg
	enemy->movement('n',player,EntMgr);
	if(enemy->getDistance(enemy,player) < AVOID_RANGE)
	{
		enemy->setStatus(E_WALK);
		if(enemy->getDrawInfo().hitBox.top >= player->getDrawInfo().hitBox.top &&
		enemy->getDistance(enemy,player) < abs(player->getDrawInfo().hitBox.top+YRANGE_OFFSET))
		{
			enemy->movement('u',player,EntMgr);
		}
		if(enemy->getDrawInfo().hitBox.bottom <= player->getDrawInfo().hitBox.bottom &&
		enemy->getDistance(enemy,player) < abs(player->getDrawInfo().hitBox.bottom-YRANGE_OFFSET)) 
		{
			enemy->movement('d',player,EntMgr);
		}
		if(enemy->getDrawInfo().hitBox.left >= player->getDrawInfo().hitBox.left  &&
		enemy->getDistance(enemy,player) < abs(player->getDrawInfo().hitBox.left))
		{
			enemy->movement('r',player,EntMgr);
		}
		if(enemy->getDrawInfo().hitBox.right <= player->getDrawInfo().hitBox.right &&
		enemy->getDistance(enemy,player) < abs(player->getDrawInfo().hitBox.right)) 
		{
			enemy->movement('l',player,EntMgr);
		}
	}
	if(enemy->getDistance(enemy,player) >= AVOID_RANGE)
	{
		//Make a Wait Here so the enemy looks like he is thinking before reacting
		enemy->ChangeState(Chase::Instance());
	}
	printf("Running away from player still!\n");
}

void RunAway::Exit(Enemy *enemy)
{
	printf("Leaving RunAway State\n");
}