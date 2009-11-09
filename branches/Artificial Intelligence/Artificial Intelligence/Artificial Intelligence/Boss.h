#include "BaseGameEntity.h"
#include "State.h"
#include "Status.h"

class State;

class Boss : protected BaseGameEntity
{
private:
	status_type status;
	State *CurrentState;
	Vector2D velocity;
	int health,power,stat,
		val,special,sPower;
	bool alive;
public:

	Boss(int ID);
	~Boss() {}

	int getHealth() {return health;}
	int getSpecialMeter() {return special;}
	int getPower() {return power;}
	int getsPower() {return sPower;}
	
	bool isAlive();
	virtual void UpdateStat(int stat, int val);
	virtual void UpdateState();
	//virtual void setImg(/* *DirectXSurface */);
	//virtual char getName();

	void ChangeState(State* pNewState);
	void ChangeStatus(status_type sts) {status = sts;}
	status_type getStatus() {return status;}
};