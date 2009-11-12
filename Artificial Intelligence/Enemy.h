#include "BaseGameEntity.h"
#include "State.h"
#include "Status.h"

class Enemy : public BaseGameEntity
{
private:
	status_type status;
	State<Enemy> *CurrentState;
	/* attributes omitted */
public:
	Enemy();
	virtual void Update();
	void ChangeState(State<Enemy>* pNewState);
	void ChangeStatus(status_type sts) {status = sts;}
	status_type getStatus() {return status;}
};