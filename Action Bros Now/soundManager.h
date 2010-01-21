//singleton class to play sound effects& BGM

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <windows.h>
#include <dsound.h>
#include "dsutil.h"
#include <string>

#include "soundContainer.h"

using namespace std;
//??????????????????
//#include <dxerr.h>

class soundManager
{
private:
	//singleton functions
	static soundManager* instance;
	soundManager();

	//direct sound vars
	LPDIRECTSOUND8			g_pDS;		//DirectSound Device

	int curVolume;
	int volChange;

	//the container
	soundContainer sCont;

	//loads single wav
	//need to code
	//need sname as a string to load the file
	bool loadWAV(char* sName);

public:
	//singelton function
	static soundManager* getInstance();

	//direct sound functions
	bool initSound(HWND hwnd);
	void shutDown();
	
	//need to code
	//load sNames as strings
	bool loadAllSounds();

	//these will need to be given a string wavName
	//need to code
	void playSound();
	void playSoundLoop(char* sound);
	void stopSound(); 
};

#endif 
