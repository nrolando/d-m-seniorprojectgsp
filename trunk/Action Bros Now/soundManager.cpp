#include "soundManager.h"

soundManager* soundManager::instance = 0;

soundManager::soundManager()
{
}

soundManager* soundManager::getInstance()
{
	if(instance == 0)
		instance = new soundManager();

	return instance;
}