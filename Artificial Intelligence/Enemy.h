#include "BaseGameEntity.h"
#include "Status.h"

class State;

class Enemy : public BaseGameEntity
{
private:
	status_type status;
	State *CurrentState;
	/* attributes omitted */
public:
	Enemy();
	virtual void Update();
	void ChangeState(State* pNewState);
	void ChangeStatus(status_type sts) {status = sts;}
	status_type getStatus() {return status;}
};