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

	if(enemy->getDistance(enemy->getPos(),player->getPos()) <= CHASE_RANGE)
	{
		printf("Within Range...changing state");
		enemy->ChangeState(Chase::Instance());
	}
	else if(enemy->getDistance(enemy->getPos(),player->getPos()) <= ATTACK_RANGE)
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

void Chase::Execute(Enemy* enemy, Player* player,std::vector<BaseGameEntity*> EntMgr)
{
	//If player is within attack range then switch to Attack state
	if(enemy->getDistance(enemy->getPos(),player->getPos()) <= ATTACK_RANGE && !enemy->isTagged())
			enemy->ChangeState(Attack::Instance());
	if(!enemy->isTagged()){
		if(enemy->getPos().y > player->getPos().y+RANGE_OFFSET)
		{
			enemy->setStatus(E_WALK);
			enemy->movement('d',player->getPos(),EntMgr);
		}
		else if(enemy->getPos().y < player->getPos().y-RANGE_OFFSET)
		{
			enemy->setStatus(E_WALK);
			enemy->movement('u',player->getPos(),EntMgr);
		}
		else if(enemy->getPos().x > player->getPos().x+RANGE_OFFSET)
		{
			enemy->setStatus(E_WALK);
			enemy->movement('l',player->getPos(),EntMgr);
		}
		else if(enemy->getPos().x < player->getPos().x)
		{
			enemy->setStatus(E_WALK);
			enemy->movement('r',player->getPos(),EntMgr);
		}
	}
	else
	{
		enemy->movement('n',player->getPos(),EntMgr);
		enemy->setStatus(E_IDLE);
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
	
	if(enemy->getDistance(enemy->getPos(),player->getPos()) <= ATTACK_RANGE &&
	   enemy->getDistance(enemy->getPos(),player->getPos()) >= (ATTACK_RANGE/2)-RANGE_OFFSET)
	{	
			if(player->isStunned())
				++attacks;
			if(attacks >= 10)
			{
				attacks = 0;
				enemy->ChangeState(RunAway::Instance());	
			}
			else if(enemy->getDistance(enemy->getPos(),player->getPos()) <= ATTACK_RANGE/2 &&
			enemy->getPos().y >= player->getPos().y-RANGE_OFFSET ||
			enemy->getPos().y <= player->getPos().y+RANGE_OFFSET)
			{
				++attacks;
				enemy->movement('n',player->getPos(),EntMgr);
				enemy->setStatus(E_ATTACK1);
			}
	}
	//else if(enemy->getDistance(enemy->getPos(),player->getPos()) < ATTACK_RANGE/4)
	//{
	//	if(player->getDrawInfo().hitBox.right < enemy->getPos().x)
	//	{
	//		enemy->setStatus(E_WALK);
	//		enemy->movement('l',player->getPos(),EntMgr);
	//	}
	//	else if(player->getPos().x > enemy->getPos().x)
	//	{
	//		enemy->setStatus(E_WALK);
	//		enemy->movement('r',player->getPos(),EntMgr);
	//	}
	//}
	else if(enemy->getDistance(enemy->getPos(),player->getPos()) > ATTACK_RANGE)
	{
		enemy->movement('n',player->getPos(),EntMgr);
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
	if(enemy->getDistance(enemy->getPos(),player->getPos()) < SOLDIER1_AVOID_RANGE)
	{
		enemy->setStatus(E_WALK);
		if(enemy->getPos().y >= player->getPos().y &&
		enemy->getDistance(enemy->getPos(),player->getPos()) < SOLDIER1_AVOID_RANGE)
		{
			enemy->movement('u',player->getPos(),EntMgr);
		}
		else if(enemy->getPos().y <= player->getPos().y-RANGE_OFFSET &&
		enemy->getDistance(enemy->getPos(),player->getPos()) < SOLDIER1_AVOID_RANGE) 
		{
			enemy->movement('d',player->getPos(),EntMgr);
		}
		else if(enemy->getPos().x < player->getPos().x &&
		enemy->getDistance(enemy->getPos(),player->getPos()) < SOLDIER1_AVOID_RANGE)
		{
			enemy->movement('r',player->getPos(),EntMgr);
		}
		else if(enemy->getPos().x > player->getPos().x+RANGE_OFFSET &&
		enemy->getDistance(enemy->getPos(),player->getPos()) < SOLDIER1_AVOID_RANGE) 
		{
			enemy->movement('l',player->getPos(),EntMgr);
		}
		else
			enemy->movement('u',player->getPos(),EntMgr);
	}
	else if(enemy->getDistance(enemy->getPos(),player->getPos()) >= SOLDIER1_AVOID_RANGE)
		enemy->ChangeState(Chase::Instance());
	printf("Running away from player still!\n");
}

void RunAway::Exit(Enemy *enemy)
{
	printf("Leaving RunAway State\n");
}