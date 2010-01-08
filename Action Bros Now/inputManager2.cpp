#include "inputManager2.h"

InputManager2::InputManager2(HINSTANCE hInstance, HWND wndHandle)
{
	inputflag = 0;
	downflag  = 0;
	lastinput = 0;
	locked = false;

	comboStart = 0;
	buffIterator = 0;

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

void InputManager2::setInput(clock_t time)
{
	hr = g_lpDIDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);

	lastinput = inputflag;

	if(KEYDOWN(buffer, DIK_LEFT))
	{
		inputflag |= INPUT_LEFT;
		if(comboStart == 0)
			comboStart = time;

		comboBuffer[buffIterator] = 'l';
			buffIterator++;
	
	}
	else
		inputflag = inputflag &(~INPUT_LEFT);

	if(KEYDOWN(buffer, DIK_RIGHT))
	{
		inputflag |= INPUT_RIGHT;
		if(comboStart == 0)
			comboStart = time;

		comboBuffer[buffIterator] = 'r';
			buffIterator++;
	
	}
	else
		inputflag = inputflag &(~INPUT_RIGHT);

	if(KEYDOWN(buffer, DIK_UP))
	{
		inputflag |= INPUT_UP;
		if(comboStart == 0)
			comboStart = time;

		comboBuffer[buffIterator] = 'u';
			buffIterator++;
	}
	else
		inputflag = inputflag &(~INPUT_UP);

	if(KEYDOWN(buffer, DIK_DOWN))
	{
		inputflag |= INPUT_DOWN;
		if(comboStart == 0)
			comboStart = time;

		comboBuffer[buffIterator] = 'd';
			buffIterator++;

	}
	else
		inputflag = inputflag &(~INPUT_DOWN);

	if(KEYDOWN(buffer, DIK_Z))  //player punch action
	{
		inputflag |= INPUT_Z;
		if(comboStart == 0)
			comboStart = time;

		comboBuffer[buffIterator] = 'p';
			buffIterator++;
	}
	else
		inputflag = inputflag &(~INPUT_Z);

	if(KEYDOWN(buffer, DIK_X)) //player kick action
	{
		inputflag |= INPUT_X;
		if(comboStart == 0)
			comboStart = time;

		comboBuffer[buffIterator] = 'k';
			buffIterator++;
	}
	else
		inputflag = inputflag &(~INPUT_X);

	if(KEYDOWN(buffer, DIK_C)) //player special action
	{
		inputflag |= INPUT_C;
		if(comboStart == 0)
			comboStart = time;

		comboBuffer[buffIterator] = 's';
			buffIterator++;
	}
	else
		inputflag = inputflag &(~INPUT_C);

	downflag = (inputflag^lastinput)&inputflag;
}

char InputManager2::getInput(clock_t now)
{
	//THIS SHOULD WORK CANNOT BREAK POINT TEST
	//NEED TO PROPERLY AUGMENT LOCKED SYSTEM AND GET AN ANIMATION 
	//FOR COMBOS IN ORDER TO TEST PROPERLY
	//either the combo window has closed or the buff is full
	if(comboStart > 0 && now - comboStart >= COMBO_TIME || buffIterator > 2)
	{
		comboStart = 0;
		buffIterator = 0;
		//check if combo is correct
		char temp = comboCheck();
		//if correct return the combo's char
		if(temp != 'i')
			return temp;
	}
	//these are diagonal movement
	else if(inputflag & INPUT_UP && inputflag & INPUT_RIGHT)
	{
		char tempC = charReturn();
		if(tempC != 'i')
			return tempC;
		else
			return 'w';
	}
	if(inputflag & INPUT_UP && inputflag & INPUT_LEFT)
	{
		char tempC = charReturn();
		if(tempC != 'i')
			return tempC;
		else
			return 'x';
	}
	if(inputflag & INPUT_DOWN && inputflag & INPUT_RIGHT)
	{
		char tempC = charReturn();
		if(tempC != 'i')
			return tempC;
		else
			return 'y';
	}
	if(inputflag & INPUT_DOWN && inputflag & INPUT_LEFT)
	{
		char tempC = charReturn();
		if(tempC != 'i')
			return tempC;
		else
			return 'z';
	}

	//Player movement//
	if(inputflag & INPUT_UP)
	{
		char tempC = charReturn();
		if(tempC != 'i')
			return tempC;
		else
			return 'u'; //move player up
	}
	else if(inputflag & INPUT_DOWN)
	{
		char tempC = charReturn();
		if(tempC != 'i')
			return tempC;
		else
			return 'd'; //move player down
	}
	else if(inputflag & INPUT_LEFT)
	{
		char tempC = charReturn();
		if(tempC != 'i')
			return tempC;
		else
			return 'l'; //move player left
	}
	else if(inputflag & INPUT_RIGHT)
	{
		char tempC = charReturn();
		if(tempC != 'i')
			return tempC;
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

char InputManager2::comboCheck()
{
	int check = -1;
	//compare the buffer to the predefined combos
	for(int i = 0; i < 2; i++)
	{
		if(check == -1)//if no match keep looking
		{
			for(int j = 0; j < 3; j++)
			{
				if(comboBuffer[j] == comboDefinitions[i][j])
					check = i;
				else
					check = -1;
			}
		}
	}
	//return a char based on check
	//either a combo char or 'i' (nothing)
	switch(check)
	{
	case 0:
		return '1';
	case 1:
		return '2';
	default:
		return 'i';
	}
}

char InputManager2::charReturn()
{
	//player attacks while moving//
		if(downflag & INPUT_Z)
			return 'p';
		else if(downflag & INPUT_X)
			return 'k';
		else if(downflag & INPUT_C)
			return 's';
		else
			return 'i';
}