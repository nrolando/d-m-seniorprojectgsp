#include "Player.h"
#include <iostream>

Player::Player():BaseGameEntity("Hero name here")
{
	health = 100;
	special = 0;
	pPower = 20;
	kPower = 10;
	sPower = 80;
	lives = 3;
	vel = D3DXVECTOR3(3.0f,3.0f,0.0f);
	state = IDLE;
	sprInfo.width = sprInfo.height = 128;

	/*sprInfo Initialization*/
//initializing shouldn't be here. should be initialized elsewhere so that multiple players can be made
	//sprInfo->POS = D3DXVECTOR3(50.0f,300.0f,0.0f);
}

Player::Player(std::string n):BaseGameEntity(n)
{
	health = 100;
	special = 0;
	pPower = 20;
	kPower = 10;
	sPower = 80;
	lives = 3;
	vel = D3DXVECTOR3(3.0f,3.0f,0.0f);
	state = IDLE;
	now = stunStart = aniFStart = 0;

	/*sprInfo Initialization*/
//initializing for player sprites shouldn't be done here. check Player::initSprInfo()
	//sprInfo->POS = D3DXVECTOR3(50.0f,300.0f,0.0f);
	
}
/*
void Player::initSprInfo(int w, int h, D3DXVECTOR3 pos)
{
	sprInfo->ss_ptr = spriteContainer::getInstance()->SS_getElem(0);
	sprInfo->width = w;
	sprInfo->height = h;
	sprInfo->POS = pos;

}
*/
bool Player::actionPossible(char input)
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
	if(input == 'l')
	{
		vel.x = -1.0f;
		if(state == IDLE)
		{
			anim = 0;
		}
		state = WALK;
	}
	else if(input == 'r')
	{
		vel.x = 1.0f;
		if(state == IDLE)
		{
			anim = 0;
		}
		state = WALK;
	}
	else
	{
		state = IDLE;
		vel.x = vel.y = vel.z = 0.0f;
	}

	/*//set a value for vel in constructor
	switch(state)
	{
	case STUN:
		//don't do anything!
		break;
	case IDLE:
		//move to left, change to walking
		if(input == 'l')
		{
			POS.x -= vel.x;
			if(state == IDLE)
			state = WALK;
			anim = 0;
		}
		//move to right, change to walking
		else if(input == 'r')
		{
			POS.x += vel.x;
			state = WALK;
			anim = 0;
		}
		//if no button pressed go to IDLE
		else if(input == 'i')
			state = IDLE;
		break;
	case WALK:
		if(input == 'l')
			POS.x -= vel.x;
		//move to right, change to walking
		else if(input == 'r')
			POS.x += vel.x;
		//if no button pressed go to IDLE
		else if(input == 'i')
			state = IDLE;
		break;
	//ADD CASES FOR ATTACKs
	}*/
}

void Player::UpdateState(clock_t time)
{
	now = clock();

	switch(state)
	{
	case STUN:
		//if has been long enough
		if(now - stunStart >= STUNTIME)
		{
			//switch to first fram of idle state
			state = IDLE;
			anim = 0;
			aniFStart = now;
			//make sure state enum value matches up with location on sprite sheet!
			//set the rect to the correct area of the sprite sheet
			this->calcDrawRECT();
		}
		//if still stunned and time to switch frame of animation
		else if(now - aniFStart >= ANIMATIONGAP)
		{
			//make sure state enum value matches up with location on sprite sheet!
			//set the rect to the correct area of the sprite sheet
			this->calcDrawRECT();

			//loop to the beginning of the animation
			if(anim == MAXSTUNFRAME)
				anim = 0;
			//advance 1 frame
			else
				anim++;
			aniFStart = now;
		}
		break;
	case IDLE:
		//if time to switch frame of animation
		if(now - aniFStart >= ANIMATIONGAP)
		{
			//make sure state enum value matches up with location on sprite sheet!
			//set the rect to the correct area of the sprite sheet
			this->calcDrawRECT();

			//loop to the beginning of animation
			if(anim == MAXIDLEFRAME)
				anim = 0;
			//advance 1 frame
			else
				anim++;
			aniFStart = now;
		}
		break;
	case WALK:
		//if time to switch frame of animation
		if(now - aniFStart >= ANIMATIONGAP)
		{
			//make sure state enum value matches up with location on sprite sheet!
			//set the rect to the correct area of the sprite sheet
			this->calcDrawRECT();

			//loop to the beginning of animation
			if(anim == MAXWALKFRAME)
				anim = 0;
			//advance 1 frame
			else
				anim++;
			aniFStart = now;
		}
		break;
	}
	this->move();
}

/*
void Player::setSheetPtr(spriteSheet *tempInfo)
{
	sprInfo->ss_ptr = tempInfo;

	sprInfo->drawRect.top = 0;
	sprInfo->drawRect.left = 0;
	sprInfo->drawRect.right = (anim * sprInfo->drawRect.left) + sprInfo->width;
	sprInfo->drawRect.bottom = sprInfo->drawRect.top + sprInfo->height;
}
*/
bool Player::isAlive()
{
	if(health > 0 && health <= 200)
		return true;
	else
		return false;
}