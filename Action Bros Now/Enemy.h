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
#define FALLFRAME			6
#define STUNFRAME			1

#define DEATHFRAMETIME		150
#define WALKFRAMETIME		150

class Enemy : public BaseGameEntity
{
private:
	status_type status;
protected:
	State<Enemy, Player> *CurrentState;
	
	/* Attributes - power moved to BGE*/
	int stat,val;

public:
	Enemy(int ID);
	Enemy(int ID, char KEY, D3DXVECTOR3 pos, spriteSheet *ptr);
	~Enemy() {}

	/* Updates for HP/MP and States*/
	virtual void calcDrawRECT();
	virtual void UpdateStat(int,int);
	virtual void UpdateState(Player*,std::vector<BaseGameEntity*>);
	virtual void stun();
	virtual void stun(int);

	int  getDistance(Enemy*,Player*);
	int	 getDistance(Enemy*,RECT);

	bool update();
	void rotate()			{ faceRight = !faceRight; }
	virtual void die();

	/*   Attribute related functions   */
	void setHealth(int hp) { health = hp;}
	int  getHealth()       { return health;}
	bool isRotated()	   { return faceRight;}
	bool MovementPossible(std::vector<BaseGameEntity*>);

	/* Enemy State Updates */
	void ChangeState(State<Enemy, Player>* pNewState);
	void ChangeStatus(status_type sts) {status = sts;}
	void ChasePlayer(Player*);
	void AvoidEntity(Player*,std::vector<BaseGameEntity*>);
	
	void movement(char,Player*,std::vector<BaseGameEntity*>);
	status_type getStatus() {return status;}
};

#endif