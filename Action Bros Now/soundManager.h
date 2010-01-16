////singleton class to play sound effects& BGM
//
//#ifndef SOUNDMANAGER_H
//#define SOUNDMANAGER_H
//
//#include <windows.h>
//#include <dsound.h>
//#include "dsuti.h"
//#include <string>
//using namespace std;
////??????????????????
////#include <dxerr.h>
//
//class soundManager
//{
//private:
//	//singleton functions
//	static soundManager* instance;
//	soundManager();
//
//	//direct sound vars
//	LPDIRECTSOUND8			g_pDS;		//DirectSound Device
//	LPDIRECTSOUNDBUFFER		DSBuffer;	//DirectSound Buffer (make separate container)
//
//	int curVolume;
//	int volChange;
//
//public:
//	//singelton function
//	static soundManager* getInstance();
//
//	//direct sound functions
//	bool initSound(HWND hwnd);
//	void shutDown();
//	LPDIRECTSOUNDBUFFER loadWAVToBuffer(std::string wavName); //put in container class
//
//
//	//these will need to be given a string wavName
//	void playSound();
//	void playSoundLoop();
//	void stopSound(); 
//};
//
//#endif 
