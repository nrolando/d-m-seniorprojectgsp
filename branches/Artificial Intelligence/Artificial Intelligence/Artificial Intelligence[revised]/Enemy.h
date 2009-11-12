#ifndef ENEMY_H
#define ENEMY_H

#include "BaseGameEntity.h"
#include "State.h"
#include "Status.h"

class Enemy : protected BaseGameEntity
{
	private:
		status_type status;
	protected:
		Vector2D velocity;
		State<Enemy> *CurrentState;
		int health,power,stat,val;
		bool alive;
	public:
		Enemy(int ID);
		~Enemy() {}

	/*   Attribute related functions   */

	int setHealth(int hp) {health = hp;}
	int getHealth() {return health;}
	bool isAlive();

	int setPower(int p) {power = p;}
	int getPower() {return power;}

	Vector2D setVelocity(Vector2D v) {velocity = v;}
	Vector2D getVelocity() {return velocity;}

	virtual void UpdateStat(int stat, int val);
	virtual void UpdateState();
	//virtual void setImg(/* *DirectXSurface */);
	//virtual char getName();

	void ChangeState(State<Enemy>* pNewState);
	void ChangeStatus(status_type sts) {status = sts;}
	status_type getStatus() {return status;}
};

#endif