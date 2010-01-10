#ifndef ENEMY_H
#define ENEMY_H

#include "BaseGameEntity.h"
#include "State.h"
#include "Status.h"

#define STUNTIME			500
#define CSWALKFRAME			3	//number of walking frames
#define CSDIEFRAME			3

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
		virtual void calcDrawRECT();

		virtual void UpdateStat(int stat, int val);
		virtual void UpdateState();

		bool update();

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