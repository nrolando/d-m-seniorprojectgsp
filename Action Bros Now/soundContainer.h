//#ifndef SOUNDCONTAINER_H
//#define SOUNDCONTAINER_H
//
//#include "common.h"
//#include <fstream>
//#include <string>
//#include <vector>
//
//using namespace std;
//
//class soundContainer
//{
//private:
//	//singleton stuff
//	static soundContainer* instance;
//	soundContainer();
//
//	//list of all the sounds
//	std::vector<soundFile*> soundCont;
//
//	//loads individual WAVz
//	LPDIRECTSOUNDBUFFER LoadWav(char* sName);
//
//public:
//	//singlton function
//	static soundContainer* getInstance();
//
//	//need an init function to get a ptr to the sound device from sound container
//	//or move the loadstuff to th manager...
//
//	void loadAllSounds();
//	LPDIRECTSOUNDBUFFER* getSound(std::string sName);
//	
//
//};
//
//#endif