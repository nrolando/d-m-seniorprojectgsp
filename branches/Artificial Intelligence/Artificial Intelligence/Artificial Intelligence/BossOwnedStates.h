#include "State.h"

//class Boss;
class Enemy;

//Boss Idle State
class Idle : public State<Enemy>
{
private:
	Idle() {}

	//copy ctor and assignment should be private
	Idle(const Idle&);
	Idle& operator=(const Idle&);
public:
	virtual void Enter(Enemy* boss);
	virtual void Exit(Enemy* boss);
	virtual void Execute(Enemy* boss);

	//this is a singleton
	static Idle* Instance();
};
//
////Boss Chase State
//class Chase : public State<Boss>
//{
//private:
//	Chase() {}
//
//	//copy ctor and assignment should be private
//	Chase(const Chase&);
//	Chase& operator=(const Chase&);
//public:
//	virtual void Enter(Boss* boss);
//	virtual void Exit(Boss* boss);
//	virtual void Execute(Boss* boss);
//
//	//this is a singleton
//	static Chase* Instance();
//};
//
////Boss Attack State
//class Attack : public State<Boss>
//{
//private:
//	Attack() {}
//
//	//copy ctor and assignment should be private
//	Attack(const Attack&);
//	Attack& operator=(const Attack&);
//public:
//	virtual void Enter(Boss* boss);
//	virtual void Exit(Boss* boss);
//	virtual void Execute(Boss* boss);
//
//	//this is a singleton
//	static Attack* Instance();
//};
//
////Boss RunAway State
//class RunAway : public State<Boss>
//{
//private:
//	RunAway() {}
//
//	//copy ctor and assignment should be private
//	RunAway(const RunAway&);
//	RunAway& operator=(const RunAway&);
//public:
//	virtual void Enter(Boss* boss);
//	virtual void Exit(Boss* boss);
//	virtual void Execute(Boss* boss);
//
//	//this is a singleton
//	static RunAway* Instance();
//};