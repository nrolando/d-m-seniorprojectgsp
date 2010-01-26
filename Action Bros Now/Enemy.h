#ifndef ENEMY_H
#define ENEMY_H

#include "BaseGameEntity.h"
#include "State.h"
#include "Status.h"

#define STUNTIME			500
#define IDLEANIMATION		180
#define ATTACKFRAME			4
#define CSWALKFRAME			6	//number of walking frames
#define IDLEFRAME           6
#define CSDIEFRAME			6
#define STUNFRAME			1

#define CSWALKFRAMETIME		150

class Enemy : public BaseGameEntity
{
	private:
		status_type status;
	protected:
		State<Enemy> *CurrentState;
		bool rotated;
		
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
		virtual void UpdateState(D3DXVECTOR3);
		virtual void stun();

		bool update();

		/*   Attribute related functions   */
		int setHealth(int hp) {health = hp;}
		int getHealth() {return health;}

		int setPower(int p) {power = p;}
		int getPower() {return power;}
		bool isRotated() {return rotated;}

		/* Enemy State Updates */
		void ChangeState(State<Enemy>* pNewState);
		void ChangeStatus(status_type sts) {status = sts;}
		void movement(char);
		void rotate()			{rotated = !rotated;}
		status_type getStatus() {return status;}
};

#endif