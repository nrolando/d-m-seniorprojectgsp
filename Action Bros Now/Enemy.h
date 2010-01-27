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

class Player;

class Enemy : public BaseGameEntity
{
	private:
		status_type status;
	protected:
		State<Enemy,Player> *CurrentState;
		bool rotated;
		
		/* Attributes */
		int stat,val;

public:
	Enemy(int ID);
	Enemy(int ID, char KEY, D3DXVECTOR3 pos, spriteSheet *ptr);
	~Enemy() {}


	/* Updates for HP/MP and States*/
	virtual void calcDrawRECT();
	virtual void UpdateStat(int stat, int val);
	virtual void UpdateState(Player*);
	virtual void stun();

	bool update();

	/*   Attribute related functions   */
	void setHealth(int hp) {health = hp;}
	int getHealth() {return health;}
	void setPower(int p) {power = p;}
	int getPower() {return power;}

	/* Enemy State Updates */
	void ChangeState(State<Enemy,Player>* pNewState);
	void ChangeStatus(status_type sts) {status = sts;}
	int getStatus() {return status;}
	void movement(char);
};

#endif