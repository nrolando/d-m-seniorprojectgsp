#ifndef STATE_H
#define STATE_H

#include "common.h"
class BaseGameEntity;

template <class entity_type, class player>
class State
{
public:
	virtual ~State() {}
	virtual void Enter(entity_type*)= 0;
	virtual void Execute(entity_type*,player*,std::vector<BaseGameEntity*>)= 0;
	virtual void Exit(entity_type*)= 0;
};

#endif