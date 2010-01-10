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

 spriteSheet* spriteContainer::getElemKey(char key)
 {
	 for(unsigned int i = 0; i < spriteCont.size(); ++i)
	 {
		 if(spriteCont[i].key == key)
			return &spriteCont[i];
	 }
	 return NULL;
 }