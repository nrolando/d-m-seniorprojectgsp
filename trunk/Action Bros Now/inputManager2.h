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
	//iterator for combos
	int cbIter;
	char comboBuff[4];

	char buffer[256];		//for the DI stuff

	//COMBO MEMBERS***************
	//for the combo window
	clock_t comboStart;
	//iterator for combos
	int iter;
	char comboCheck(char);
	//COMBO MEMBERS***************

	//makes getInput smaller 
	char charReturn();

public:
	InputManager2(HINSTANCE hInstance, HWND wndHandle);
	~InputManager2();

	//for switching windows
	void acquireDevice()				{g_lpDIDevice->Acquire();}
	void unAcquireDevice()				{g_lpDIDevice->Unacquire();}

	void setInput();
	//passing screen advancement
	char getInput(int, bool);
};
#endif