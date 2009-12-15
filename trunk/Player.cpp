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
	//set a value for vel in constructor
	switch(state)
	{
	case STUN:
		//don't do anything!
		break;
	case IDLE:
		//move to left, change to walking
		if(input == 'L')
		{
			POS.x -= vel.x;
			if(state == IDLE)
			state = WALK;
			sprInfo->framenumber = 0;
		}
		//move to right, change to walking
		else if(input == 'R')
		{
			POS.x += vel.x;
			state = WALK;
			sprInfo->framenumber = 0;
		}
		//if no button pressed go to IDLE
		else if(input == 'I')
			state = IDLE;
		break;
	case WALK:
		if(input == 'L')
			POS.x -= vel.x;
		//move to right, change to walking
		else if(input == 'R')
			POS.x += vel.x;
		//if no button pressed go to IDLE
		else if(input == 'I')
			state = IDLE;
		break;
	//ADD CASES FOR ATTACKs
	}
}

void Player::UpdateState(clock_t time)
{
	now = time;

	switch(state)
	{
	case STUN:
		//if has been long enough
		if(now - stunStart >= STUNTIME)
		{
			//switch to first fram of idle state
			state = IDLE;
			sprInfo->framenumber = 0;
			aniFStart = now;
			//make sure state enum value matches up with location on sprite sheet!
			//set the rect to the correct area of the sprite sheet
			sprInfo->drawRect.top = IDLE * SPRITE_HEIGHT;
			sprInfo->drawRect.left = sprInfo->framenumber * SPRITE_WIDTH;
			sprInfo->drawRect.right = sprInfo->drawRect.left + SPRITE_WIDTH;//sprInfo->framenumber * (sprInfo->drawRect.left + SPRITE_WIDTH);
			sprInfo->drawRect.bottom = sprInfo->drawRect.top + SPRITE_HEIGHT;
		}
		//if still stunned and time to switch frame of animation
		else if(now - aniFStart >= ANIMATIONGAP)
		{
			//loop to the beginning of the animation
			if(sprInfo->framenumber == MAXSTUNFRAME)
				sprInfo->framenumber = 0;
			//advance 1 frame
			else
				sprInfo->framenumber++;
			aniFStart = now;
			//make sure state enum value matches up with location on sprite sheet!
			//set the rect to the correct area of the sprite sheet
			sprInfo->drawRect.top = STUN * SPRITE_HEIGHT;
			sprInfo->drawRect.left = sprInfo->framenumber * SPRITE_WIDTH;
			sprInfo->drawRect.right = sprInfo->drawRect.left + SPRITE_WIDTH;//sprInfo->framenumber * (sprInfo->drawRect.left + SPRITE_WIDTH);
			sprInfo->drawRect.bottom = sprInfo->drawRect.top + SPRITE_HEIGHT;
		}
		break;
	case IDLE:
		//if time to switch frame of animation
		if(now - aniFStart >= ANIMATIONGAP)
		{
			//loop to the beginning of animation
			if(sprInfo->framenumber == MAXIDLEFRAME)
				sprInfo->framenumber = 0;
			//advance 1 frame
			else
				sprInfo->framenumber++;
			aniFStart = now;
			//make sure state enum value matches up with location on sprite sheet!
			//set the rect to the correct area of the sprite sheet
			sprInfo->drawRect.top = IDLE * SPRITE_HEIGHT;
			sprInfo->drawRect.left = sprInfo->framenumber * SPRITE_WIDTH;
			sprInfo->drawRect.right = sprInfo->drawRect.left + SPRITE_WIDTH;//sprInfo->framenumber * (sprInfo->drawRect.left + SPRITE_WIDTH);
			sprInfo->drawRect.bottom = sprInfo->drawRect.top + SPRITE_HEIGHT;
		}
		break;
	case WALK:
		//if time to switch frame of animation
		if(now - aniFStart >= ANIMATIONGAP)
		{
			//loop to the beginning of animation
			if(sprInfo->framenumber == MAXWALKFRAME)
				sprInfo->framenumber = 0;
			//advance 1 frame
			else
				sprInfo->framenumber++;
			aniFStart = now;
			//make sure state enum value matches up with location on sprite sheet!
			//set the rect to the correct area of the sprite sheet
			sprInfo->drawRect.top = WALK * SPRITE_HEIGHT;
			sprInfo->drawRect.left = sprInfo->framenumber * SPRITE_WIDTH;
			sprInfo->drawRect.right = sprInfo->drawRect.left + SPRITE_WIDTH;//sprInfo->framenumber * (sprInfo->drawRect.left + SPRITE_WIDTH);
			sprInfo->drawRect.bottom = sprInfo->drawRect.top + SPRITE_HEIGHT;
		}
		break;
	}
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