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
	vel = D3DXVECTOR3(3.0f,3.0f,0.0f);
	state = IDLE;
	now = stunStart = aniFStart = animTime = 0;
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

PlayerStates Player::DoAction(char input)
{
	if(input == 'l')
	{
		vel.x = -speed;
		vel.y = 0.0f;
		vel.z = 0.0f;
		if(state == IDLE)
			anim = 0;
		state = WALK;

		return WALK;
	}
	else if(input == 'r')
	{
		vel.x = speed;
		vel.y = 0.0f;
		vel.z = 0.0f;
		if(state == IDLE)
			anim = 0;
		state = WALK;

		return WALK;
	}
	else if(input == 'd')
	{
		vel.x = 0.0f;
		vel.y = -speed;
		vel.z = 0.0f;
		if(state == IDLE)
			anim = 0;
		state = WALK;

		return WALK;
	}
	else if(input == 'u')
	{
		vel.x = 0.0f;
		vel.y = speed;
		vel.z = 0.0f;
		if(state == IDLE)
			anim = 0;
		state = WALK;

		return WALK;
	}
	else if(input == 'w')
	{
		vel.x = speed;
		vel.y = speed;
		vel.z = 0.0f;
		if(state == IDLE)
			anim = 0;
		state = WALK;

		return WALK;
	}
	//up & left
	else if(input == 'x')
	{
		vel.x = -speed;
		vel.y = speed;
		vel.z = 0.0f;
		if(state == IDLE)
			anim = 0;
		state = WALK;

		return WALK;
	}
	else if(input == '1')
	{
		return WALK;
	}
	//down & right
	else if(input == 'y')
	{
		vel.x = speed;
		vel.y = -speed;
		vel.z = 0.0;
		if(state == IDLE)
			anim = 0;
		state = WALK;

		return WALK;
	}
	//down & left
	else if(input == 'z')
	{
		vel.x = -speed;
		vel.y = -speed;
		vel.z = 0.0;
		if(state == IDLE)
			anim = 0;
		state = WALK;

		return WALK;
	}
	else if(input == 'p')
	{
		//the player stops to kick
		vel.x = vel.y = vel.z = 0.0f;
//this checks if the animation is done, if not, nothing happens
//note: all player input action should be done inside this if statement
		if(state != PUNCH && animTime <= clock() - animStartTime)
		{
		//set the animation time
		animTime = (MAXPUNCHFRAME) * ANIMATIONGAP;
		animStartTime = clock();
		anim = 0;
		state = PUNCH;
		//set hit frames
		hitFrames[0] = 1;
		hitFrames[1] = -1;
		hitFrames[2] = -1;
		}
		return ATTACK;
	}
	else if(input == 'k')
	{
		//the player stops to kick
		vel.x = vel.y = vel.z = 0.0f;
//this checks if the animation is done, if not, nothing happens
//note: all player input action should be done inside this if statement
		if(state != KICK && animTime <= clock() - animStartTime)
		{
			//set the animation time
			animTime = (MAXKICKFRAME) * ANIMATIONGAP;
			animStartTime = clock();
			anim = 0;
			state = KICK;
			//set hit frames
			hitFrames[0] = 3;
			hitFrames[1] = -1;
			hitFrames[2] = -1;
		}
		return ATTACK;
	}
	else
	{
		if(animTime <= clock() - animStartTime)	//or anything that's not supposed to idle itself
		{
			state = IDLE;
			lastAttFrame = -1;		//reset
		}
		vel.x = vel.y = vel.z = 0.0f;
	}

	return IDLE;
}

int Player::UpdatePlayerState()
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
				this->calcDrawRECT(state);
			}
			//if still stunned and time to switch frame of animation
			else if(now - aniFStart >= ANIMATIONGAP)
			{
				//make sure state enum value matches up with location on sprite sheet!
				//set the rect to the correct area of the sprite sheet
				this->calcDrawRECT(state);

				//loop to the beginning of the animation
				if(anim >= MAXSTUNFRAME-1)
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
				this->calcDrawRECT(state);

				//loop to the beginning of animation
				if(anim >= MAXIDLEFRAME-1)
					anim = 0;
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
				this->calcDrawRECT(state);

				//loop to the beginning of animation
				if(anim >= MAXWALKFRAME-1)
					anim = 1;
				//advance 1 frame
				else
					anim++;
				aniFStart = now;
			}
			break;

		case PUNCH:
			//if time to switch frame of animation
			if(now - aniFStart >= ANIMATIONGAP)
			{
				this->calcDrawRECT(state);

				if(anim < MAXPUNCHFRAME-1)
					anim++;
				else
				{
					anim = 0;
					return 1;
				}
				aniFStart = now;
			}			
			break;

		case KICK:
			if(now - aniFStart >= ANIMATIONGAP)
			{
				this->calcDrawRECT(state);

				if(anim < MAXKICKFRAME-1)
					anim++;
				else
				{
					anim = 0;
					return 1;
				}

				aniFStart = now;
			}
			break;
	}

	return 0;
}

void Player::calcDrawRECT(int state)
{
	sprInfo.drawRect.left = anim * sprInfo.width;
	sprInfo.drawRect.right = sprInfo.drawRect.left + sprInfo.width;
	sprInfo.drawRect.top = state * sprInfo.height;
	sprInfo.drawRect.bottom = sprInfo.drawRect.top + sprInfo.height;

	//Player's hitBox for dmg verification
	sprInfo.hitBox.top  = long(sprInfo.POS.y);
	sprInfo.hitBox.left = long(sprInfo.POS.x);
	sprInfo.hitBox.right = sprInfo.hitBox.left + 41;
	sprInfo.hitBox.bottom  = sprInfo.hitBox.top + 80;

	//Player's threatBox for dmg verification
	//while in DEBUG this will be shown
	if(state == PUNCH)
	{
		sprInfo.threatBox.top  = long(sprInfo.POS.y);
		sprInfo.threatBox.left = long(sprInfo.POS.x);
		sprInfo.threatBox.right = sprInfo.threatBox.left + 75;
		sprInfo.threatBox.bottom  = sprInfo.threatBox.top + 80;
	}
	else if(state == KICK)
	{
		sprInfo.threatBox.top  = long(sprInfo.POS.y);
		sprInfo.threatBox.left = long(sprInfo.POS.x);
		sprInfo.threatBox.right = sprInfo.threatBox.left + 90;
		sprInfo.threatBox.bottom  = sprInfo.threatBox.top + 80;
	}
	else
	{
		sprInfo.threatBox.top  = 0;
		sprInfo.threatBox.left = 0;
		sprInfo.threatBox.right = 0;
		sprInfo.threatBox.bottom  = 0;
	}
}

bool Player::isAlive()
{
	if(health > 0 && health <= 200)
		return true;
	else
		return false;
}