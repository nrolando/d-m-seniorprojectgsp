#include "Player.h"
#include <iostream>

Player::Player(std::string n):BaseGameEntity(n)
{
	health = 100;
	maxHealth = 100;
	special = 100;
	maxSpecial = 100;
	lives = 3;
	vel = D3DXVECTOR3(0.0f,0.0f,0.0f);
	state = IDLE;
	score = 0;
}

void Player::UpdateStat(int stat, int val)
{
	switch(stat)
	{//dont change alive to false until death animation has finished, not currently being implemented,
	//waiting for death animation
		case 0:
			health += val;
			if(health < 0)
				health = 0;
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
		}
		else if(state == RUN)
		{
			vel.x = -speed * 2.0f;
			vel.y = -(speed * 2.0f);
			vel.z = 0.0f;
			faceRight = false;
			//set hit frames
			hitFrames[0] = -1;
			hitFrames[1] = -1;
			hitFrames[2] = -1;

			state = RUN;
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
		}
	}
	else if(input == 'p')
	{
//this checks if the animation is done, if not, nothing happens
//note: all player input action should be done inside this if statement
		if(state == IDLE || state == WALK || state == RUN)
		{
			soundManager::getInstance()->playSound("sword_swing");
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
			//play sfx
			soundManager::getInstance()->playSound("sword_swing");
		}
	}
	else if(input == 'k')
	{
//this checks if the animation is done, if not, nothing happens
//note: all player input action should be done inside this if statement
		if(state == RUN)
		{
			anim = 0;
			aniFStart = now;
			state = KICK2;
			lastAttFrame = -1;
			//set hit frames
			hitFrames[0] = 2;
			hitFrames[1] = 4;
			hitFrames[2] = -1;
			//play sfx
			soundManager::getInstance()->playSound("sword_swing");
		}
		else if(state == IDLE || state == WALK)
		{
			soundManager::getInstance()->playSound("sword_swing");
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
			//play sfx
			soundManager::getInstance()->playSound("sword_swing");
		}
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
	case KICK2:
		if(now - aniFStart >= MAXKICKANIMATION)
		{
			if(anim < MAXKICK2FRAME-1)
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
		if(now - stunStart >= stunTime)
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
				anim = 0;
			aniFStart = now;
		}
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
	sprInfo.hitBox.top  = long(sprInfo.POS.y - 48);
	sprInfo.hitBox.left = long(sprInfo.POS.x + 45);
	sprInfo.hitBox.right = sprInfo.hitBox.left + 45;
	sprInfo.hitBox.bottom  = sprInfo.hitBox.top - 80;

	//Player's threatBox for dmg verification
	//while in DEBUG this will be shown
	//PLEASE NOTE: THIS IS ALL FOR RIGHT FACING ONLY! LEFT FACE IS NOT SET UP YET
	if(state == PUNCH)
	{
		if(faceRight)
		{
			sprInfo.threatBox.top  = long(sprInfo.POS.y - 45);
			sprInfo.threatBox.left = long(sprInfo.POS.x + 78);
			sprInfo.threatBox.right = sprInfo.threatBox.left + 35;
			sprInfo.threatBox.bottom  = sprInfo.threatBox.top - 25;
		}
		else
		{
			sprInfo.threatBox.top  = long(sprInfo.POS.y - 45);
			sprInfo.threatBox.left = long(sprInfo.POS.x + 15);
			sprInfo.threatBox.right = sprInfo.threatBox.left + 35;
			sprInfo.threatBox.bottom  = sprInfo.threatBox.top - 25;
		}
	}
	else if(state == KICK)
	{
		if(faceRight)
		{
			sprInfo.threatBox.top  = long(sprInfo.POS.y - 55);
			sprInfo.threatBox.left = long(sprInfo.POS.x + 80);
			sprInfo.threatBox.right = sprInfo.threatBox.left + 48;
			sprInfo.threatBox.bottom  = sprInfo.threatBox.top - 30;
		}
		else
		{
			sprInfo.threatBox.top  = long(sprInfo.POS.y - 55);
			sprInfo.threatBox.left = long(sprInfo.POS.x + 5);
			sprInfo.threatBox.right = sprInfo.threatBox.left + 48;
			sprInfo.threatBox.bottom  = sprInfo.threatBox.top - 30;
		}
	}
	else if(state == COMBO1)
	{
		if(anim >= 16)	//flame hitFrame
		{
			if(faceRight)
			{
				//can add individual threat boxes for each hitFrame[] if needed!
				sprInfo.threatBox.top  = long(sprInfo.POS.y - 25);
				sprInfo.threatBox.left = long(sprInfo.POS.x + 68);
				sprInfo.threatBox.right = sprInfo.threatBox.left + 42;
				sprInfo.threatBox.bottom  = sprInfo.threatBox.top - 75;
			}
			else
			{
				sprInfo.threatBox.top  = long(sprInfo.POS.y - 25);
				sprInfo.threatBox.left = long(sprInfo.POS.x + 18);
				sprInfo.threatBox.right = sprInfo.threatBox.left + 42;
				sprInfo.threatBox.bottom  = sprInfo.threatBox.top - 75;
			}
		}
		else
		{
			if(faceRight)
			{
				//can add individual threat boxes for each hitFrame[] if needed!
				sprInfo.threatBox.top  = long(sprInfo.POS.y - 40);
				sprInfo.threatBox.left = long(sprInfo.POS.x + 78);
				sprInfo.threatBox.right = sprInfo.threatBox.left + 32;
				sprInfo.threatBox.bottom  = sprInfo.threatBox.top - 50;
			}
			else
			{
				sprInfo.threatBox.top  = long(sprInfo.POS.y - 40);
				sprInfo.threatBox.left = long(sprInfo.POS.x + 18);
				sprInfo.threatBox.right = sprInfo.threatBox.left + 32;
				sprInfo.threatBox.bottom  = sprInfo.threatBox.top - 50;
			}
		}
	}
	else if(state == KICK2)
	{
		if(faceRight)
		{
			sprInfo.threatBox.top  = long(sprInfo.POS.y - 68);
			sprInfo.threatBox.left = long(sprInfo.POS.x + 75);
			sprInfo.threatBox.right = sprInfo.threatBox.left + 52;
			sprInfo.threatBox.bottom  = sprInfo.threatBox.top - 25;
		}
		else
		{
			sprInfo.threatBox.top  = long(sprInfo.POS.y - 68);
			sprInfo.threatBox.left = long(sprInfo.POS.x + 1);
			sprInfo.threatBox.right = sprInfo.threatBox.left + 52;
			sprInfo.threatBox.bottom  = sprInfo.threatBox.top - 25;
		}
	}
	else
	{
		sprInfo.threatBox.top  = -8880;
		sprInfo.threatBox.left = -8880;
		sprInfo.threatBox.right = -8880;
		sprInfo.threatBox.bottom = -8880;
	}
}

void Player::stun()
{
	//min + rand() % max - min + 1
	stunTime = 100 + rand() % 201;
	stunStart = clock();
	state = STUN;
	this->setVel(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

void Player::die()
{
}

int Player::getDmg()
{
	switch(state)
	{
	case PUNCH:
		return P_POWER;
		break;
	case KICK:
	case KICK2:
		return K_POWER;
		break;
	case COMBO1:
		if(anim == hitFrames[0])
			return P_POWER;
		else if(anim == hitFrames[1])
			return K_POWER;
		else if(anim == hitFrames[2])
			return C1_POWER;
		break;
	default:
		return 0;
	};
	return 0;
}