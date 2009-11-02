#include <cassert>
#include "EnemyOwnedStates.h"
#include "Enemy.h"

Enemy::Enemy():status(InRange),
			   CurrentState(Patrol::Instance())
{}

void Enemy::Update()
{
	CurrentState->Execute(this);
}

void Enemy::ChangeState(State* pNewState)
{
	CurrentState->Exit(this);
	CurrentState = pNewState;
	CurrentState->Enter(this);
}