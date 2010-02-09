#include "State.h"

class Boss;
class Player;
class EntityManager;
//class Enemy;

//Boss Aggresive State
class Stationary: public State<Boss,Player>
{
private:
	Stationary() {}

	//copy ctor and assignment should be private
	Stationary(const Stationary&);
	Stationary& operator=(const Stationary&);
public:
	virtual void Enter(Boss* boss);
	virtual void Exit(Boss* boss);
	virtual void Execute(Boss* boss, Player* player,std::vector<BaseGameEntity*> EntMgr);

	//this is a singleton
	static Stationary* Instance();
};


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
	virtual void Execute(Boss* boss, Player* player,std::vector<BaseGameEntity*> EntMgr);

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
	virtual void Execute(Boss* boss, Player* player,std::vector<BaseGameEntity*> EntMgr);
	virtual void Exit(Boss* boss);

	//this is a singleton
	static Defensive* Instance();
};

//Boss Beserk State
class Beserk : public State<Boss,Player>
{
private:
	bool taunted;
	Beserk() {taunted = false;}

	//copy ctor and assignment should be private
	Beserk(const Beserk&);
	Beserk& operator=(const Beserk&);
public:
	virtual void Enter(Boss* boss);
	virtual void Exit(Boss* boss);
	virtual void Execute(Boss* boss, Player* player,std::vector<BaseGameEntity*> EntMgr);

	//this is a singleton
	static Beserk* Instance();
};

//Boss Attacking State
class Attacking : public State<Boss,Player>
{
private:
	int attacks;
	Attacking() {attacks = 0;}

	//copy ctor and assignment should be private
	Attacking(const Attacking&);
	Attacking& operator=(const Attacking&);
public:
	virtual void Enter(Boss* boss);
	virtual void Exit(Boss* boss);
	virtual void Execute(Boss* boss, Player* player,std::vector<BaseGameEntity*> EntMgr);

	//this is a singleton
	static Attacking* Instance();
};


//Boss Rush State
class Rush : public State<Boss,Player>
{
private:
	int attacks;
	Rush() {attacks = 0;}

	//copy ctor and assignment should be private
	Rush(const Rush&);
	Rush& operator=(const Rush&);
public:
	virtual void Enter(Boss* boss);
	virtual void Exit(Boss* boss);
	virtual void Execute(Boss* boss, Player* player,std::vector<BaseGameEntity*> EntMgr);

	//this is a singleton
	static Rush* Instance();
};

//Boss Rush State
class Avoid : public State<Boss,Player>
{
private:
	Avoid() {}

	//copy ctor and assignment should be private
	Avoid(const Avoid&);
	Avoid& operator=(const Avoid&);
public:
	virtual void Enter(Boss* boss);
	virtual void Exit(Boss* boss);
	virtual void Execute(Boss* boss, Player* player,std::vector<BaseGameEntity*> EntMgr);

	//this is a singleton
	static Avoid* Instance();
};