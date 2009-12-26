#include "inputManager2.h"

InputManager2::InputManager2(HINSTANCE hInstance, HWND wndHandle)
{
	inputflag = 0;
	downflag  = 0;
	lastinput = 0;

	//create the DI object
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
							IID_IDirectInput8, (void**)&g_lpDI, NULL);
	//retreve ptr to DI device interface
	hr = g_lpDI->CreateDevice(GUID_SysKeyboard, &g_lpDIDevice, NULL);

	hr = g_lpDIDevice->SetDataFormat(&c_dfDIKeyboard);

	//set the cooperative lvl
	hr = g_lpDIDevice->SetCooperativeLevel(wndHandle,
										DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//get access to the input device
	hr = g_lpDIDevice->Acquire();
}

InputManager2::~InputManager2()
{
	if (g_lpDI) 
    { 
        if (g_lpDIDevice) 
        { 
        // Always unacquire device before calling Release(). 
            g_lpDIDevice->Unacquire(); 
            g_lpDIDevice->Release();
            g_lpDIDevice = NULL; 
        } 
        g_lpDI->Release();
        g_lpDI = NULL; 
    } 
}

void InputManager2::setInput()
{
	hr = g_lpDIDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);

	lastinput = inputflag;

	if(KEYDOWN(buffer, DIK_LEFT))
		inputflag |= INPUT_LEFT;
	else
		inputflag = inputflag &(~INPUT_LEFT);

	if(KEYDOWN(buffer, DIK_RIGHT))
		inputflag |= INPUT_RIGHT;
	else
		inputflag = inputflag &(~INPUT_RIGHT);

	if(KEYDOWN(buffer, DIK_UP))
		inputflag |= INPUT_UP;
	else
		inputflag = inputflag &(~INPUT_UP);

	if(KEYDOWN(buffer, DIK_DOWN))
		inputflag |= INPUT_DOWN;
	else
		inputflag = inputflag &(~INPUT_DOWN);

	downflag = (inputflag^lastinput)&inputflag;
}

char InputManager2::getInput()
{

	/////////////////////
	//the movement keys//
	/////////////////////

	//these are diagonal movement
	if((KEYDOWN(buffer, DIK_UP)) & (KEYDOWN(buffer, DIK_RIGHT)))
	{
		//wxyz
		return 'w';
	}
	if((KEYDOWN(buffer, DIK_UP)) & (KEYDOWN(buffer, DIK_LEFT)))
	{
		return 'x';
	}
	if(inputflag & INPUT_DOWN & INPUT_RIGHT)
	{
		return 'y';
	}
	if(inputflag & INPUT_DOWN & INPUT_LEFT)
	{
		return 'z';
	}

	//these are normal movement
	if(inputflag & INPUT_UP)
	{
		//move up
		return 'u';
	}
	if(inputflag & INPUT_DOWN)
	{
		//move down
		return 'd';
	}
	if(inputflag & INPUT_LEFT)
	{
		//move left
		return 'l';
	}
	if(inputflag & INPUT_RIGHT)
	{
		//move right
		return 'r';
	}
	
	else
		return 'i';
}