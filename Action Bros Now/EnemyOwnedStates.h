#include "State.h"

class Enemy;
class Player;

//Enemy Idle State
class Idle : public State<Enemy,Player>
{
private:
	Idle() {}

	//copy ctor and assignment should be private
	Idle(const Idle&);
	Idle& operator=(const Idle&);
public:
	virtual void Enter(Enemy* enemy);
	virtual void Exit(Enemy* enemy);
	virtual void Execute(Enemy* enemy, Player* player);

	//this is a singleton
	static Idle* Instance();
};

//Enemy Patrol State
class Patrol : public State<Enemy,Player>
{
private:
	Patrol() {}

	//copy ctor and assignment should be private
	Patrol(const Patrol&);
	Patrol& operator=(const Patrol&);
public:
	virtual void Enter(Enemy* enemy);
	virtual void Exit(Enemy* enemy);
	virtual void Execute(Enemy* enemy, Player* player);

	//this is a singleton
	static Patrol* Instance();
};

//Enemy Chase State
class Chase : public State<Enemy,Player>
{
private:
	Chase() {}

	//copy ctor and assignment should be private
	Chase(const Chase&);
	Chase& operator=(const Chase&);
public:
	virtual void Enter(Enemy* enemy);
	virtual void Exit(Enemy* enemy);
	virtual void Execute(Enemy* enemy, Player* player);

	//this is a singleton
	static Chase* Instance();
};

//Enemy Attack State
class Attack : public State<Enemy,Player>
{
private:
	Attack() {}

	//copy ctor and assignment should be private
	Attack(const Attack&);
	Attack& operator=(const Attack&);
public:
	virtual void Enter(Enemy* enemy);
	virtual void Exit(Enemy* enemy);
	virtual void Execute(Enemy* enemy, Player* player);

	//this is a singleton
	static Attack* Instance();
};

//Enemy RunAway State
class RunAway : public State<Enemy,Player>
{
private:
	RunAway() {}

	//copy ctor and assignment should be private
	RunAway(const RunAway&);
	RunAway& operator=(const RunAway&);
public:
	virtual void Enter(Enemy* enemy);
	virtual void Exit(Enemy* enemy);
	virtual void Execute(Enemy* enemy, Player* player);

	//this is a singleton
	static RunAway* Instance();
};

