#include "inputManager2.h"

InputManager2::InputManager2(HINSTANCE hInstance, HWND wndHandle)
{
	inputflag = 0;
	downflag  = 0;
	lastinput = 0;
	locked = false;

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
	if(KEYDOWN(buffer, DIK_Z))  //player punch action
		inputflag |= INPUT_Z;
	else
		inputflag = inputflag &(~INPUT_Z);

	if(KEYDOWN(buffer, DIK_X)) //player kick action
		inputflag |= INPUT_X;
	else
		inputflag = inputflag &(~INPUT_X);

	if(KEYDOWN(buffer, DIK_C)) //player special action
		inputflag |= INPUT_C;
	else
		inputflag = inputflag &(~INPUT_C);

	downflag = (inputflag^lastinput)&inputflag;
}

char InputManager2::getInput()
{
	//these are diagonal movement
	if(inputflag & INPUT_UP && inputflag & INPUT_RIGHT)
	{
		//player attacks while moving//
		if(inputflag & INPUT_Z)
			return 'p';
		else if(inputflag & INPUT_X)
			return 'k';
		else if(inputflag & INPUT_C)
			return 's';
		else
			return 'w';
	}
	if(inputflag & INPUT_UP && inputflag & INPUT_LEFT)
	{
		//player attacks while moving//
		if(inputflag & INPUT_Z)
			return 'p';
		else if(inputflag & INPUT_X)
			return 'k';
		else if(inputflag & INPUT_C)
			return 's';
		else
			return 'x';
	}
	if(inputflag & INPUT_DOWN && inputflag & INPUT_RIGHT)
	{
		//player attacks while moving//
		if(inputflag & INPUT_Z)
			return 'p';
		else if(inputflag & INPUT_X)
			return 'k';
		else if(inputflag & INPUT_C)
			return 's';
		else
			return 'y';
	}
	if(inputflag & INPUT_DOWN && inputflag & INPUT_LEFT)
	{
		//player attacks while moving//
		if(inputflag & INPUT_Z)
			return 'p';
		else if(inputflag & INPUT_X)
			return 'k';
		else if(inputflag & INPUT_C)
			return 's';
		else
			return 'z';
	}

	//Player movement//
	if(inputflag & INPUT_UP)
	{
		//player attacks while moving//
		if(inputflag & INPUT_Z)
			return 'p';
		else if(inputflag & INPUT_X)
			return 'k';
		else if(inputflag & INPUT_C)
			return 's';
		else
			return 'u'; //move player up
	}
	else if(inputflag & INPUT_DOWN)
	{
		//player attacks while moving//
		if(inputflag & INPUT_Z)
			return 'p';
		else if(inputflag & INPUT_X)
			return 'k';
		else if(inputflag & INPUT_C)
			return 's';
		else
			return 'd'; //move player down
	}
	else if(inputflag & INPUT_LEFT)
	{
		//player attacks while moving//
		if(inputflag & INPUT_Z)
			return 'p';
		else if(inputflag & INPUT_X)
			return 'k';
		else if(inputflag & INPUT_C)
			return 's';
		else
			return 'l'; //move player left
	}
	else if(inputflag & INPUT_RIGHT)
	{
		//player attacks while moving//
		if(inputflag & INPUT_Z)
			return 'p';
		else if(inputflag & INPUT_X)
			return 'k';
		else if(inputflag & INPUT_C)
			return 's';
		else			//else if only movement
			return 'r';
	}
	
	//Input for Player Attack Actions//
	if(downflag & INPUT_Z)
	{
		//move camera right
		return 'p';
	}
	
	if(downflag & INPUT_X)
	{
		//move camera right
		return 'k';
	}
	
	if(downflag & INPUT_C)
	{
		//move camera right
		return 's';
	}

	else
		return 'i';
}

