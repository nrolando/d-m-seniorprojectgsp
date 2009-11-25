#include "spriteContainer.h"

//init static instance
spriteContainer* spriteContainer::instance = NULL;

spriteContainer::spriteContainer()
{
}

spriteContainer::getInstance()
{
	if(instance == NULL)
		instance = new spriteContainer();

	return
		instance;
}