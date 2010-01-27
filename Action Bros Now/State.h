#ifndef STATE_H
#define STATE_H

#include "common.h"

template <class entity_type, class player>
class State
{
public:
	virtual ~State() {}
	virtual void Enter(entity_type*)= 0;
	virtual void Execute(entity_type*,player*)= 0;
	virtual void Exit(entity_type*)= 0;
};

#endif