#ifndef ENEMY_H
#define ENEMY_H

#include "BaseGameEntity.h"
#include "State.h"
#include "Status.h"

class Enemy : public BaseGameEntity
{
	private:
		status_type status;
	protected:
		State<Enemy> *CurrentState;
		
		/* Attributes */
		bool alive;
		int health,power,stat,val;
		Vector2D velocity;

	public:
		Enemy(int ID);
		Enemy(int ID, char KEY, Vector2D pos, Sprite *ptr);
		~Enemy() {}

		bool isAlive();

		/* Updates for HP/MP and States*/
		virtual void UpdateStat(int stat, int val);
		virtual void UpdateState(clock_t);

		/*   Attribute related functions   */
		int setHealth(int hp) {health = hp;}
		int getHealth() {return health;}

		int setPower(int p) {power = p;}
		int getPower() {return power;}

		Vector2D setVelocity(Vector2D v) {velocity = v;}
		Vector2D getVelocity() {return velocity;}

		/* Enemy State Updates */
		void ChangeState(State<Enemy>* pNewState);
		void ChangeStatus(status_type sts) {status = sts;}
		status_type getStatus() {return status;}
};

#endif