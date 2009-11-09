#include "State.h"

//Enemy Patrol State
class Patrol : public State
{
private:
	Patrol() {}

	//copy ctor and assignment should be private
	Patrol(const Patrol&);
	Patrol& operator=(const Patrol&);
public:
	virtual void Enter(Enemy* boss);
	virtual void Exit(Enemy* boss);
	virtual void Execute(Enemy* boss);

	//this is a singleton
	static Patrol* Instance();
};

//Enemy Chase State
class Chase : public State
{
private:
	Chase() {}

	//copy ctor and assignment should be private
	Chase(const Chase&);
	Chase& operator=(const Chase&);
public:
	virtual void Enter(Enemy* boss);
	virtual void Exit(Enemy* boss);
	virtual void Execute(Enemy* boss);

	//this is a singleton
	static Chase* Instance();
};

//Enemy Attack State
class Attack : public State
{
private:
	Attack() {}

	//copy ctor and assignment should be private
	Attack(const Attack&);
	Attack& operator=(const Attack&);
public:
	virtual void Enter(Enemy* boss);
	virtual void Exit(Enemy* boss);
	virtual void Execute(Enemy* boss);

	//this is a singleton
	static Attack* Instance();
};

//Enemy RunAway State
class RunAway : public State
{
private:
	RunAway() {}

	//copy ctor and assignment should be private
	RunAway(const RunAway&);
	RunAway& operator=(const RunAway&);
public:
	virtual void Enter(Enemy* boss);
	virtual void Exit(Enemy* boss);
	virtual void Execute(Enemy* boss);

	//this is a singleton
	static RunAway* Instance();
};