#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <queue>
#include <string>
#include <time.h>
#include <windows.h>
#include <iostream>

#define COMBO_TIME 1000

using namespace std;

class inputManager
{
private:
	//singleton stuff
	static inputManager* instance;
	inputManager();

	queue<char> inputBuffer;
	char Carray[3];
	clock_t curTime;
	clock_t prevTime;
	bool combo;
	bool timesUP;

public:
	//singleton acess
	static inputManager* getInstance();
	void setInput(WPARAM, clock_t);
	char eval_ret();

	bool isEmpty();
	bool isTimeUP()		{return timesUP;}
};

#endif