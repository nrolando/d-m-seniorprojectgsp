#ifndef INPUTMANAGER2_H
#define INPUTMANAGER2_H

#include <windows.h>
#include <dinput.h>
#include <time.h>

#include "common.h"

//#define DIRECTINPUT_VERSION 0x800


class InputManager2
{
private:
	LPDIRECTINPUT8			g_lpDI;			//direct input object
	LPDIRECTINPUTDEVICE8	g_lpDIDevice;	//direct input device
	HRESULT hr;

	//binary flags for the inputs
	int inputflag;
	int downflag;
	int lastinput;

	//iterator for the inBuffer
	int buffIterator;

	char comboBuffer[3];	//for combos
	char buffer[256];		//for the DI stuff
	bool locked;

	//for the combo window
	clock_t comboStart;

	char comboCheck();
	//makes getInput smaller 
	char charReturn();

public:
	InputManager2(HINSTANCE hInstance, HWND wndHandle);
	~InputManager2();

	void setInput(clock_t);
	char getInput(clock_t);

	//going to need to look at this and mess with 
	//for combo stuff
	void unlock() {locked = false;}
	void lock() {locked = true;}
	bool isLocked() {return locked;}

};
#endif