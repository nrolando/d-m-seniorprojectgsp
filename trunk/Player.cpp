#include "Player.h"
#include <iostream>

Player::Player(std::string n):BaseGameEntity(n)
{
	health = 100;
	special = 0;
	pPower = 20;
	kPower = 10;
	sPower = 80;
	lives = 3;
	vel = D3DXVECTOR3(0.0f,0.0f,0.0f);
	state = IDLE;
}

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

void Player::DoAction(char input)
{
	switch(input)
	{
		case 'u':
			vel.y -= 3.0f;
			break;
		case 'd':
			vel.y += 3.0f;
			break;
		case 'l':
			vel.x -= 3.0f;
			break;
		case 'r':
			vel.x += 3.0f;
			break;
	}

	switch(state)
	{
		case IDLE:
			sprInfo->drawRect.top = IDLE * SPRITE_HEIGHT;
			sprInfo->drawRect.left = IDLE;
			sprInfo->drawRect.right = sprInfo->framenumber * (sprInfo->drawRect.left + SPRITE_WIDTH);
			sprInfo->drawRect.bottom = sprInfo->drawRect.top + SPRITE_HEIGHT;
			break;
		case WALK:
			sprInfo->drawRect.top = WALK * SPRITE_HEIGHT;
			sprInfo->drawRect.left = 0;
			sprInfo->drawRect.right = sprInfo->framenumber * (sprInfo->drawRect.left + SPRITE_WIDTH);
			sprInfo->drawRect.bottom = sprInfo->drawRect.top + SPRITE_HEIGHT;
			break;
		case ATTACK:
			sprInfo->drawRect.top = 2 * SPRITE_HEIGHT;
			sprInfo->drawRect.left = 0;
			sprInfo->drawRect.right = sprInfo->framenumber * (sprInfo->drawRect.left + SPRITE_WIDTH);
			sprInfo->drawRect.bottom = sprInfo->drawRect.top + SPRITE_HEIGHT;

			sprInfo->drawRect.top = 3 * SPRITE_HEIGHT;
			sprInfo->drawRect.left = 0;
			sprInfo->drawRect.right = sprInfo->framenumber * (sprInfo->drawRect.left + SPRITE_WIDTH);
			sprInfo->drawRect.bottom = sprInfo->drawRect.top + SPRITE_HEIGHT;

			break;
		case SPECIAL:
			
			break;
		case STUN:
			
			break;
	}
}

void Player::UpdateState(clock_t time)
{
}


void Player::setSheetPtr(spriteSheet *tempInfo)
{
	sprInfo->spriteSheet->gSprite = tempInfo->gSprite;
	sprInfo->spriteSheet->gTexture = tempInfo->gTexture;

	sprInfo->drawRect.top = 0;
	sprInfo->drawRect.left = 0;
	sprInfo->drawRect.right = sprInfo->framenumber * (sprInfo->drawRect.left + SPRITE_WIDTH);
	sprInfo->drawRect.bottom = sprInfo->drawRect.top + SPRITE_HEIGHT;

	sprInfo->POS = vel;
}

bool Player::isAlive()
{
	if(health > 0 && health <= 200)
		return true;
	else
		return false;
}