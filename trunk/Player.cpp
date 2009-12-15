#include "Player.h"
#include <iostream>

Player::Player(int ID):BaseGameEntity(ID)
{}

bool Player::actionPossible(D3DXVECTOR3 pos)
{
	//Code to check if player new position 
	//is greater than the player's walking area
	//or if the player has collided with something
	return true;
}

void Player::UpdateStat(int stat, int val)
{
	switch(stat)
	{
		case 0:
			health += val;
			break;
		case 1:
			special += val;
			break;
		default:
			printf("incorrect stat!\n");
			break;
	}
}

void Player::UpdateState()
{
}

bool Player::isAlive()
{
	if(health > 0 && health <= 200)
		return true;
	else
		return false;
}