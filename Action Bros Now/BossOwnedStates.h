#include "State.h"

class Boss;
class Player;
class Enemy;

//Boss LessThanFifty State
class LessThanFifty : public State<Boss,Player>
{
private:
	LessThanFifty() {}

	//copy ctor and assignment should be private
	LessThanFifty(const LessThanFifty&);
	LessThanFifty& operator=(const LessThanFifty&);
public:
	virtual void Enter(Boss* boss);
	virtual void Exit(Boss* boss);
	virtual void Execute(Boss* boss, Player* player);

	//this is a singleton
	static LessThanFifty* Instance();
};

//Boss LessThanTwentyFive State
class LessThanTwentyFive : public State<Boss,Player>
{
private:
	LessThanTwentyFive() {}

	//copy ctor and assignment should be private
	LessThanTwentyFive(const LessThanTwentyFive&);
	LessThanTwentyFive& operator=(const LessThanTwentyFive&);
public:
	virtual void Enter(Boss* boss);
	virtual void Exit(Boss* boss);
	virtual void Execute(Boss* boss, Player* player);

	//this is a singleton
	static LessThanTwentyFive* Instance();
};

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