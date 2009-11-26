#include "Status.h"
#include "State.h"
#include "Enemy.h"
#include <string>

class Boss : public Enemy
{
private:
	//status_type status;
	Vector2D velocity;
	int health,special,sPower;
public:

	Boss(int ID,std::string name);
	~Boss() {}

	int getSpecialMeter() {return special;}
	int getsPower() {return sPower;}
	
	bool isAlive();

	virtual void UpdateStat(int stat, int val);
	
	//virtual void UpdateState();
	//virtual void setImg(/* *DirectXSurface */);
	//virtual char getName();
};