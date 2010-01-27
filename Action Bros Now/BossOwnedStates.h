#include "State.h"

class Boss;
class Player;
class Enemy;

//Boss Aggresive State
class Aggressive : public State<Boss,Player>
{
private:
	Aggressive() {}

	//copy ctor and assignment should be private
	Aggressive(const Aggressive&);
	Aggressive& operator=(const Aggressive&);
public:
	virtual void Enter(Boss* boss);
	virtual void Exit(Boss* boss);
	virtual void Execute(Boss* boss, Player* player);

	//this is a singleton
	static Aggressive* Instance();
};

//Boss Defensive State
class Defensive : public State<Boss,Player>
{
private:
	Defensive() {}

	//copy ctor and assignment should be private
	Defensive(const Defensive&);
	Defensive& operator=(const Defensive&);
public:
	virtual void Enter(Boss* boss);
	virtual void Exit(Boss* boss);
	virtual void Execute(Boss* boss, Player* player);

	//this is a singleton
	static Defensive* Instance();
};

//Boss Beserk State
class Beserk : public State<Boss,Player>
{
private:
	Beserk() {}

	//copy ctor and assignment should be private
	Beserk(const Beserk&);
	Beserk& operator=(const Beserk&);
public:
	virtual void Enter(Boss* boss);
	virtual void Exit(Boss* boss);
	virtual void Execute(Boss* boss, Player* player);

	//this is a singleton
	static Beserk* Instance();
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