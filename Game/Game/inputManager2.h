#ifndef INPUTMANAGER2_H
#define INPUTMANAGER2_H

#include <windows.h>
#include <dinput.h>
#include <time.h>

//#define DIRECTINPUT_VERSION 0x800

#define INPUT_UP	0x0001
#define INPUT_DOWN  0x0002
#define INPUT_LEFT	0x0004
#define INPUT_RIGHT 0x0008

#define KEYDOWN(name, key) (name[key] & 0x80)

#define COMBO_TIME 500

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

	char inBuffer[3];		//for combos
	char buffer[256];		//for the DI stuff

	clock_t curTime;
	clock_t prevTime;

public:
	InputManager2(HINSTANCE hInstance, HWND wndHandle);
	~InputManager2();

	void setInput();
	char getInput();

};
#endif