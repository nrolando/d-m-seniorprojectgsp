#include "State.h"

class Enemy;

//Enemy Patrol State<Enemy>
class Patrol : public State<Enemy>
{
private:
	Patrol() {}

	//copy ctor and assignment should be private
	Patrol(const Patrol&);
	Patrol& operator=(const Patrol&);
public:
	virtual void Enter(Enemy* enemy);
	virtual void Exit(Enemy* enemy);
	virtual void Execute(Enemy* enemy);

	//this is a singleton
	static Patrol* Instance();
};

//Enemy Chase State<Enemy>
class Chase : public State<Enemy>
{
private:
	Chase() {}

	//copy ctor and assignment should be private
	Chase(const Chase&);
	Chase& operator=(const Chase&);
public:
	virtual void Enter(Enemy* enemy);
	virtual void Exit(Enemy* enemy);
	virtual void Execute(Enemy* enemy);

	//this is a singleton
	static Chase* Instance();
};

//Enemy Attack State<Enemy>
class Attack : public State<Enemy>
{
private:
	Attack() {}

	//copy ctor and assignment should be private
	Attack(const Attack&);
	Attack& operator=(const Attack&);
public:
	virtual void Enter(Enemy* enemy);
	virtual void Exit(Enemy* enemy);
	virtual void Execute(Enemy* enemy);

	//this is a singleton
	static Attack* Instance();
};

//Enemy RunAway State<Enemy>
class RunAway : public State<Enemy>
{
private:
	RunAway() {}

	//copy ctor and assignment should be private
	RunAway(const RunAway&);
	RunAway& operator=(const RunAway&);
public:
	virtual void Enter(Enemy* enemy);
	virtual void Exit(Enemy* enemy);
	virtual void Execute(Enemy* enemy);

	//this is a singleton
	static RunAway* Instance();
};

