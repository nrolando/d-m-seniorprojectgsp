#ifndef SOUNDCONTAINER_H
#define SOUNDCONTAINER_H

#include "common.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class soundContainer
{
private:
	//list of all the sounds
	std::vector<soundFile> soundCont;

public:

	//make isEmpty and clear functions

	//something wrong here soundName gets lost...
	void		pushSound(soundFile sound)	{soundCont.push_back(sound);}
	soundFile*  getSound(char* soundName);

};

#endif