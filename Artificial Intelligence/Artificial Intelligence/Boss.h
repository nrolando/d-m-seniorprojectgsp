#include "Status.h"
#include "State.h"
#include "Enemy.h"

class Boss : public Enemy
{
private:
	//status_type status;
	Vector2D velocity;
	int special,sPower;
public:

	Boss(int ID);
	~Boss() {}

	int getSpecialMeter() {return special;}
	int getsPower() {return sPower;}
	
	bool isAlive();

	virtual void UpdateStat(int stat, int val);
	//virtual void UpdateState();
	//virtual void setImg(/* *DirectXSurface */);
	//virtual char getName();

	//void ChangeState(State<Boss>* pNewState);
	//void ChangeStatus(status_type sts) {status = sts;}
	//status_type getStatus() {return status;}
};