#include "Player.h"
#include <iostream>

Player::Player(std::string n):BaseGameEntity(n)
{
	health = 100;
	maxHealth = 100;
	special = 100;
	maxSpecial = 100;
	pPower = 20;
	kPower = 10;
	sPower = 80;
	lives = 3;
	vel = D3DXVECTOR3(0.0f,0.0f,0.0f);
	state = IDLE;
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
	clock_t now = clock();

	if(input == 'l')
	{
		if(state == IDLE || state == WALK)
		{
			vel.x = -speed;
			vel.y = 0.0f;
			vel.z = 0.0f;
			faceRight = false;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;
			if(state == IDLE)
			{
				aniFStart = now;
				anim = 0;
			}
			state = WALK;

			return WALK;
		}
		else if(state == RUN)
		{
			vel.x = -(speed * 2.0f);
			vel.y = 0.0f;
			vel.z = 0.0f;
			faceRight = false;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;

			state = RUN;

			return RUN;
		}
	}
	else if(input == 'r')
	{
		if(state == IDLE || state == WALK)
		{
			vel.x = speed;
			vel.y = 0.0f;
			vel.z = 0.0f;
			faceRight = true;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;
			if(state == IDLE)
			{
				aniFStart = now;
				anim = 0;
			}
			state = WALK;

			return WALK;
		}
		//if player is running, keep running
		else if(state == RUN)
		{
			vel.x = speed * 2.0f;
			vel.y = 0.0f;
			vel.z = 0.0f;
			faceRight = true;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;

			state = RUN;

			return RUN;
		}
	}
	else if(input == 'd')
	{
		if(state == IDLE || state == WALK)
		{
			vel.x = 0.0f;
			vel.y = -speed;
			vel.z = 0.0f;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;
			if(state == IDLE)
			{
				aniFStart = now;
				anim = 0;
			}
			state = WALK;

			return WALK;
		}
	}
	else if(input == 'u')
	{
		if(state == IDLE || state == WALK)
		{
			vel.x = 0.0f;
			vel.y = speed;
			vel.z = 0.0f;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;
			if(state == IDLE)
			{
				aniFStart = now;
				anim = 0;
			}
			state = WALK;

			return WALK;
		}
	}
	//up & right
	else if(input == 'w')
	{
		if(state == IDLE || state == WALK)
		{
			vel.x = speed;
			vel.y = speed;
			vel.z = 0.0f;
			faceRight = true;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;
			if(state == IDLE)
			{
				aniFStart = now;
				anim = 0;
			}
			state = WALK;

			return WALK;
		}
		else if(state == RUN)
		{
			vel.x = speed * 2.0f;
			vel.y = speed * 2.0f;
			vel.z = 0.0f;
			faceRight = true;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;

			state = RUN;

			return RUN;
		}
	}
	//up & left
	else if(input == 'x')
	{
		if(state == IDLE || state == WALK)
		{
			vel.x = -speed;
			vel.y = speed;
			vel.z = 0.0f;
			faceRight = false;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;
			if(state == IDLE)
			{
				aniFStart = now;
				anim = 0;
			}
			state = WALK;

			return WALK;
		}
		else if(state == RUN)
		{
			vel.x = -(speed * 2.0f);
			vel.y = speed * 2.0f;
			vel.z = 0.0f;
			faceRight = false;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;

			state = RUN;

			return RUN;
		}
	}
	//down & right
	else if(input == 'y')
	{
		if(state == IDLE || state == WALK)
		{
			vel.x = speed;
			vel.y = -speed;
			vel.z = 0.0;
			faceRight = true;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;
			if(state == IDLE)
			{
				aniFStart = now;
				anim = 0;
			}
			state = WALK;

			return WALK;
		}
		else if(state == RUN)
		{
			vel.x = speed * 2.0f;
			vel.y = -(speed * 2.0f);
			vel.z = 0.0f;
			faceRight = true;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;

			state = RUN;

			return RUN;
		}
	}
	//down & left
	else if(input == 'z')
	{
		if(state == IDLE || state == WALK)
		{
			vel.x = -speed;
			vel.y = -speed;
			vel.z = 0.0;
			faceRight = false;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;
			if(state == IDLE)
			{
				aniFStart = now;
				anim = 0;
			}
			state = WALK;

			return WALK;
		}
		else if(state == RUN)
		{
			vel.x = -(speed * 2.0f);
			vel.y = -(speed * 2.0f);
			vel.z = 0.0f;
			faceRight = false;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;

			state = RUN;

			return RUN;
		}
	}
	//this should make him run to the right
	else if(input == 'R')
	{
		if(state == IDLE || state == WALK)
		{
			vel.x = speed * 2.0f;
			vel.y = 0.0;
			vel.z = 0.0f;
			faceRight = true;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;
			if(state == IDLE)
			{
				aniFStart = now;
				anim = 0;
			}
			state = RUN;

			return RUN;
		}
	}
	else if(input == 'L')
	{
		if(state == IDLE || state == WALK)
		{
			vel.x = -(speed * 2.0f);
			vel.y = 0.0;
			vel.z = 0.0f;
			faceRight = false;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;
			if(state == IDLE)
			{
				aniFStart = now;
				anim = 0;
			}
			state = RUN;

			return RUN;
		}
	}
	else if(input == 'p')
	{
//this checks if the animation is done, if not, nothing happens
//note: all player input action should be done inside this if statement
		if(state == IDLE || state == WALK || state == RUN)
		{
			//the player stops to kick
			vel.x = vel.y = vel.z = 0.0f;
			anim = 0;
			aniFStart = now;
			state = PUNCH;
			lastAttFrame = -1;
			//set hit frames
			hitFrames[0] = 1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;
		}
		return ATTACK;
	}
	else if(input == 'k')
	{
//this checks if the animation is done, if not, nothing happens
//note: all player input action should be done inside this if statement
		if(state == IDLE || state == WALK || state == RUN)
		{
			//the player stops to kick
			vel.x = vel.y = vel.z = 0.0f;
			anim = 0;
			aniFStart = now;
			state = KICK;
			lastAttFrame = -1;
			//set hit frames
			hitFrames[0] = 3;
			hitFrames[1] = -1;
			hitFrames[2] = -1;
		}
		return ATTACK;
	}
	else if(input == '1')
	{
/***********	NOTE    *********
~put "!=" any states we don't want player initiating combos during
********************************/
		if(state != COMBO1)
		{
			//the player stops to kick
			vel.x = vel.y = vel.z = 0.0f;
			anim = 0;
			aniFStart = clock();
			state = COMBO1;
			lastAttFrame = -1;
			//set hit frames
			hitFrames[0] = 3;
			hitFrames[1] = 11;
			hitFrames[2] = 17;
		}
		return ATTACK;
	}
	else
	{
		if(state == WALK || state == RUN)	//or anything that's not supposed to idle itself
		{
			vel.x = vel.y = vel.z = 0.0f;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;
			aniFStart = now;
			state = IDLE;
			anim = 0;
			lastAttFrame = -1;		//reset
		}
	}
	
	return IDLE;
}

int Player::UpdatePlayerState()
{
	clock_t now = clock();
	int flag = 0;

	switch(state)
	{
	case IDLE:
		//if time to switch frame of animation
		if(now - aniFStart >= ANIMATIONGAP)
		{
			//loop to the beginning of animation
			if(anim < MAXIDLEFRAME-1)
				anim++;
			else
				anim = 0;
			aniFStart = now;
		}
		break;
	case WALK:
		//if time to switch frame of animation
		if(now - aniFStart >= ANIMATIONGAP)
		{
			//loop to the beginning of animation
			if(anim < MAXWALKFRAME-1)
				anim++;
			//advance 1 frame
			else
				anim = 1;
			aniFStart = now;
		}
		break;

	case PUNCH:
		//if time to switch frame of animation
		if(now - aniFStart >= MAXPUNCHANIMATION)
		{
			if(anim < MAXPUNCHFRAME-1)
				anim++;
			else
			{
				anim = 0;
				state = IDLE;
			}
			aniFStart = now;
		}			
		break;

	case KICK:
		if(now - aniFStart >= MAXKICKANIMATION)
		{
			if(anim < MAXKICKFRAME-1)
				anim++;
			else
			{
				anim = 0;
				state = IDLE;
			}

			aniFStart = now;
		}
		break;
	case STUN:
		//if has been long enough
		if(now - stunStart >= STUNTIME)
		{
			//switch to first fram of idle state
			state = IDLE;
			anim = 0;
			aniFStart = now;
		}
		//if still stunned and time to switch frame of animation
		else if(now - aniFStart >= ANIMATIONGAP)
		{
			//loop to the beginning of the animation
			if(anim >= MAXSTUNFRAME-1)
				anim = 0;
			//advance 1 frame
			else
				anim++;
			aniFStart = now;
		}
		break;
	case COMBO1:
		if(now - aniFStart >= MAXCOMBO1ANIMATION)
		{
			if(anim < MAXCOMBO1FRAME-1)
			{
				anim++;
		//this is used to debug: put a break point at sleep(0) to test if frames are being read
			//	if(anim == 17)
					Sleep(0);
			}
			else
			{
				anim = 0;
				state = IDLE;
			}

			aniFStart = now;
		}
		break;
	case RUN:
		//if time to switch frame of animation
		if(now - aniFStart >= ANIMATIONGAP)
		{
			//loop to the beginning of animation
			if(anim < MAXRUNFRAME-1)
				anim++;
			//advance 1 frame
			else
				anim = 1;
			aniFStart = now;
		}
		break;
	}

	this->calcDrawRECT();

	return flag;
}

void Player::calcDrawRECT()
{
	int state_frame;

	//get direction
	if(faceRight)
		state_frame = state;
	else
		state_frame = state + NUM_STATES;

	sprInfo.drawRect.left = anim * sprInfo.width;
	sprInfo.drawRect.right = sprInfo.drawRect.left + sprInfo.width;
	sprInfo.drawRect.top = state_frame * sprInfo.height;
	sprInfo.drawRect.bottom = sprInfo.drawRect.top + sprInfo.height;

	//Player's hitBox for dmg verification
	sprInfo.hitBox.top  = long(sprInfo.POS.y);
	sprInfo.hitBox.left = long(sprInfo.POS.x);
	sprInfo.hitBox.right = sprInfo.hitBox.left + 41;
	sprInfo.hitBox.bottom  = sprInfo.hitBox.top + 80;

	//Player's threatBox for dmg verification
	//while in DEBUG this will be shown
	if(state == PUNCH || state == COMBO1)
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
		sprInfo.threatBox.top  = -8880;
		sprInfo.threatBox.left = -8880;
		sprInfo.threatBox.right = -8880;
		sprInfo.threatBox.bottom = -8880;
	}
}

bool Player::isAlive()
{
	if(health > 0 && health <= 200)
		return true;
	else
		return false;
}