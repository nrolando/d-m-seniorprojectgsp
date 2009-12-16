#include "spriteContainer.h"

//init static instance
spriteContainer* spriteContainer::instance = 0;

spriteContainer::spriteContainer()
{
}

 spriteContainer* spriteContainer::getInstance()
{
	if(instance == 0)
		instance = new spriteContainer();

	return
		instance;
}