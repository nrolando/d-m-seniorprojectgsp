#include "soundContainer.h"

soundFile*  soundContainer::getSound(char *soundName)
{
	int found = -1;

	
	for(unsigned int i = 0; i < soundCont.size(); i++)
	{
		if(strcmp(soundName, soundCont[i].soundName))
			found = i;
	}

	if(found != -1)
		return &soundCont[found];
	else
		return NULL;
}