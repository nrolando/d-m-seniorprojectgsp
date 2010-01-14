#ifndef SOUND_H
#define SOUND_H

#include <dsound.h>
#include "dsutil.h"
#include <vector>

class soundManager
{
private:
	LPDIRECTSOUND8        g_pDS;	// The DirectSound Device
	std::vector<LPDIRECTSOUNDBUFFER>	SoundBuff;
	
	static soundManager* instance;
	soundManager();

public:
	static soundManager* getInstance();
};

#endif SOUND_H