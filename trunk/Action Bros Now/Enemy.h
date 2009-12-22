#ifndef ENEMY_H
#define ENEMY_H

#include "BaseGameEntity.h"
#include "State.h"
#include "Status.h"

#define CSWALKFRAME			4	//number of walking frames

//this is my temporary way of doing this til enemyownedstates are implemented
#define AN_ENEMY_STATE		0	

class Enemy : public BaseGameEntity
{
	private:
		status_type status;
	protected:
		State<Enemy> *CurrentState;
		int state_frame;
		
		/* Attributes */
		int power,stat,val;

	public:
		Enemy(int ID);
		Enemy(int ID, char KEY, D3DXVECTOR3 pos, spriteSheet *ptr);
		~Enemy() {}

		bool isAlive();

		/* Updates for HP/MP and States*/
		virtual void calcDrawRECT()
	{
		sprInfo.drawRect.left = anim * sprInfo.width;
		sprInfo.drawRect.right = sprInfo.drawRect.left + sprInfo.width;
		sprInfo.drawRect.top = state_frame * sprInfo.height;
		sprInfo.drawRect.bottom = sprInfo.drawRect.top + sprInfo.height;
	}
		virtual void UpdateStat(int stat, int val);
		virtual void UpdateState(clock_t);

		/*   Attribute related functions   */
		int setHealth(int hp) {health = hp;}
		int getHealth() {return health;}

		int setPower(int p) {power = p;}
		int getPower() {return power;}

		/* Enemy State Updates */
		void ChangeState(State<Enemy>* pNewState);
		void ChangeStatus(status_type sts) {status = sts;}
		status_type getStatus() {return status;}
};

#endif