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

void Player::DoAction(char input,InputManager2* tempInp)
{
	if(!tempInp->isLocked())
	{
		if(input == 'l')
		{
			vel.x = -speed;
			vel.y = 0.0f;
			vel.z = 0.0f;
			if(state == IDLE)
				anim = 0;
			state = WALK;
		}
		else if(input == 'r')
		{
			vel.x = speed;
			vel.y = 0.0f;
			vel.z = 0.0f;
			if(state == IDLE)
				anim = 0;
			state = WALK;
		}
		else if(input == 'd')
		{
			vel.x = 0.0f;
			vel.y = -speed;
			vel.z = 0.0f;
			if(state == IDLE)
				anim = 0;
			state = WALK;
		}
		else if(input == 'u')
		{
			vel.x = 0.0f;
			vel.y = speed;
			vel.z = 0.0f;
			if(state == IDLE)
				anim = 0;
			state = WALK;
		}
		else if(input == 'w')
		{
			vel.x = speed;
			vel.y = speed;
			vel.z = 0.0f;
			if(state == IDLE)
				anim = 0;
			state = WALK;
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
		}
		else if(input == 'p')
		{
			//the player stops to kick
			vel.x = vel.y = vel.z = 0.0f;
			if(state != PUNCH && animTime <= clock() - animStartTime)
			{
				//set the animation time
				animTime = (MAXPUNCHFRAME) * ANIMATIONGAP;
				animStartTime = clock();
				anim = 0;
				state = PUNCH;
			}
		}
		else if(input == 'k')
		{
			//the player stops to kick
			vel.x = vel.y = vel.z = 0.0f;
		
			if(state != KICK && animTime <= clock() - animStartTime)
			{
				//set the animation time
				animTime = (MAXKICKFRAME) * ANIMATIONGAP;
				animStartTime = clock();
				anim = 0;
				state = KICK;
			}
		}
		else
		{
			if(animTime <= clock() - animStartTime)	//or anything that's not supposed to idle itself
			{
				state = IDLE;
			}
			vel.x = vel.y = vel.z = 0.0f;
		}
	}
}

void Player::UpdatePlayerState(clock_t time, InputManager2* input)
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
			if(!input->isLocked())
			{
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
			}
		case WALK:
			if(!input->isLocked())
			{
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
			}
		case PUNCH:
			//if time to switch frame of animation
			if(now - aniFStart >= ANIMATIONGAP)
			{
				if(!input->isLocked())
					input->lock();

				this->calcDrawRECT(state);

				if(input->isLocked())
				{
					if(anim < MAXPUNCHFRAME-1)
						anim++;
					else
					{
						anim = 0;
						input->unlock();
					}
				}
				aniFStart = now;
			}
			break;

		case KICK:
			if(now - aniFStart >= ANIMATIONGAP)
			{
				if(!input->isLocked())
					input->lock();

				this->calcDrawRECT(state);

				if(input->isLocked())
				{
					if(anim < MAXKICKFRAME-1)
						anim++;
					else
					{
						anim = 0;
						input->unlock();
					}
				}
				aniFStart = now;
			}
			break;
	}
	this->move();
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