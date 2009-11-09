#include "BaseGameEntity.h"
#include "Status.h"

class State;

class Enemy : protected BaseGameEntity
{
private:
	status_type status;
	State *CurrentState;
	Vector2D velocity;
	int health,power,stat,val;
	bool alive;
public:
	Enemy(int ID);
	~Enemy() {}

	int getHealth() {return health;}
	int getPower() {return power;}

	bool isAlive();
	virtual void UpdateStat(int stat, int val);
	virtual void UpdateState();
	//virtual void setImg(/* *DirectXSurface */);
	//virtual char getName();

	void ChangeState(State* pNewState);
	void ChangeStatus(status_type sts) {status = sts;}
	status_type getStatus() {return status;}
};