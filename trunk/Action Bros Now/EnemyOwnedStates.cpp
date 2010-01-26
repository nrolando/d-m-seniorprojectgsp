#include "EnemyOwnedStates.h"
#include "Enemy.h"
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

void Idle::Execute(Enemy* enemy, D3DXVECTOR3 playerPos)
{
	printf("Idling...\n");

	if(enemy->getDistance(enemy->getPos(),playerPos) <= CHASE_RANGE)
	{
		printf("Within Range...changing state");
		enemy->ChangeState(Chase::Instance());
	}
	else if(enemy->getDistance(enemy->getPos(),playerPos) <= ATTACK_RANGE)
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
void Patrol::Execute(Enemy* enemy, D3DXVECTOR3 playerPos)
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

void Chase::Execute(Enemy* enemy, D3DXVECTOR3 playerPos)
{
	//If player is within attack range then switch to Attack state
	if(enemy->getDistance(enemy->getPos(),playerPos) <= ATTACK_RANGE)
			enemy->ChangeState(Attack::Instance());
	else if(enemy->getPos().y > playerPos.y+RANGE_OFFSET)
	{
		enemy->setState(E_WALK);
		enemy->movement('d');
	}
	else if(enemy->getPos().y < playerPos.y-RANGE_OFFSET) 
	{
		enemy->setState(E_WALK);
		enemy->movement('u');
	}
	else if(enemy->getPos().x > playerPos.x)
	{
		enemy->setState(E_WALK);
		enemy->movement('l');
	}
	else if(enemy->getPos().x < playerPos.x) 
	{
		enemy->setState(E_WALK);
		enemy->movement('r');
	}
	else
	{
		enemy->movement('n');
		enemy->setState(E_IDLE);
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

void Attack::Execute(Enemy* enemy, D3DXVECTOR3 playerPos)
{
	if(enemy->getDistance(enemy->getPos(),playerPos) <= ATTACK_RANGE &&
	   enemy->getDistance(enemy->getPos(),playerPos) >= (ATTACK_RANGE/2))
	{	
			enemy->movement('n');
			enemy->setState(E_ATTACK1);
			enemy->setHitFrames(2, -1, -1);
	}
	else if(enemy->getDistance(enemy->getPos(),playerPos) < ATTACK_RANGE/2)
	{
		if(playerPos.x < enemy->getPos().x)
		{
			enemy->setState(E_WALK);
			enemy->movement('r');
		}
		else if(playerPos.x < enemy->getPos().x)
		{
			enemy->setState(E_WALK);
			enemy->movement('l');
		}
	}
	else if(enemy->getDistance(enemy->getPos(),playerPos) > ATTACK_RANGE)
	{
		enemy->movement('n');
		enemy->ChangeState(Chase::Instance());
	}
	else if(enemy->getHealth() < 80)
	{
		enemy->movement('n');	
		//enemy->ChangeState(RunAway::Instance());
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

void RunAway::Execute(Enemy* enemy, D3DXVECTOR3 playerPos)
{
	
	printf("Running away from player still!\n");
}

void RunAway::Exit(Enemy *enemy)
{
	printf("Leaving RunAway State\n");
}