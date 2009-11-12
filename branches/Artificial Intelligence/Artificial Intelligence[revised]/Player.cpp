#include "Player.h"


bool Player::actionPossible(char m)
{
	return true;
}

bool Player::isAlive()
{
	if(health > 0 && health <= 200)
		return true;
	else
		return false;
}