#include "inputManager2.h"

InputManager2::InputManager2(HINSTANCE hInstance, HWND wndHandle)
{
	inputflag = 0;
	downflag  = 0;
	lastinput = 0;

	comboStartTime = 0;
	cbIter = 0;

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
	//all the else's in this function seem pointless

	hr = g_lpDIDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);

	lastinput = inputflag;

	if(KEYDOWN(buffer, DIK_LEFT))
	{
		inputflag |= INPUT_LEFT;
	}
	else
		inputflag = inputflag &(~INPUT_LEFT);

	if(KEYDOWN(buffer, DIK_RIGHT))
	{
		inputflag |= INPUT_RIGHT;
	}
	else
		inputflag = inputflag &(~INPUT_RIGHT);

	if(KEYDOWN(buffer, DIK_UP))
	{
		inputflag |= INPUT_UP;
	}
	else
		inputflag = inputflag &(~INPUT_UP);

	if(KEYDOWN(buffer, DIK_DOWN))
	{
		inputflag |= INPUT_DOWN;
	}
	else
		inputflag = inputflag &(~INPUT_DOWN);

	if(KEYDOWN(buffer, DIK_Z))  //player punch action
	{
		inputflag |= INPUT_Z;
	}
	else
		inputflag = inputflag &(~INPUT_Z);

	if(KEYDOWN(buffer, DIK_X)) //player kick action
	{
		inputflag |= INPUT_X;
	}
	else
		inputflag = inputflag &(~INPUT_X);

	if(KEYDOWN(buffer, DIK_C)) //player special action
	{
		inputflag |= INPUT_C;
	}
	else
		inputflag = inputflag &(~INPUT_C);

	if(KEYDOWN(buffer, DIK_SPACE))
	{
		inputflag |= INPUT_SPACE;
	}
	else
		inputflag = inputflag &(~INPUT_SPACE);

	downflag = (inputflag^lastinput)&inputflag;
}

char InputManager2::getInput(int screen)
{
	char combo;
	clock_t now = clock();

	switch(screen)
	{
	case 0:
		if(downflag & INPUT_Z)
			return 'p';
		break;
	case 1:
		if(downflag & INPUT_UP)
			return 'u';
		if(downflag & INPUT_DOWN)
			return 'd';
		if(downflag & INPUT_LEFT)
			return 'l';
		if(downflag & INPUT_RIGHT)
			return 'r';
		if(downflag & INPUT_Z)
			return 'p';
		if(downflag & INPUT_X)
			return 'k';
		break;
	case 2:
		//these are diagonal movement
		if(inputflag & INPUT_UP && inputflag & INPUT_RIGHT)
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
			if(downflag & INPUT_UP)
			{
				combo = comboCheck('u');
				if(combo != 'i')
					return combo;
			}

			char tempC = charReturn();
			if(tempC != 'i')
				return tempC;
			else
				return 'u'; //move player up
		}
		else if(inputflag & INPUT_DOWN)
		{
			if(downflag & INPUT_DOWN)
			{
				combo = comboCheck('d');
				if(combo != 'i')
					return combo;
			}

			char tempC = charReturn();
			if(tempC != 'i')
				return tempC;
			else
				return 'd'; //move player down
		}
		else if(inputflag & INPUT_LEFT)
		{
			if(downflag & INPUT_LEFT)
			{
				combo = comboCheck('l');
				if(combo != 'i')
					return combo;
			}

			char tempC = charReturn();
			if(tempC != 'i')
				return tempC;
			else
				return 'l'; //move player left
		}
		else if(inputflag & INPUT_RIGHT)
		{
			if(downflag & INPUT_RIGHT)
			{
				combo = comboCheck('r');
				if(combo != 'i')
					return combo;
			}

			char tempC = charReturn();
			if(tempC != 'i')
				return tempC;
			else			//else if only movement
				return 'r';
		}
		
		//Input for Player Attack Actions//
		if(downflag & INPUT_Z)
		{
			combo = comboCheck('p');
			if(combo != 'i')
				return combo;

			//move camera right
			return 'p';
		}
		
		if(downflag & INPUT_X)
		{
			combo = comboCheck('k');
			if(combo != 'i')
				return combo;

			//move camera right
			return 'k';
		}
		
		if(downflag & INPUT_C)
		{
			combo = comboCheck('s');
			if(combo != 'i')
				return combo;

			//move camera right
			return 's';
		}
		
		if(downflag & INPUT_SPACE)
		{
			return ' ';
		}
		break;
	};
	
	return 'i';
}

char InputManager2::comboCheck(char input)
{
	bool comboFlag = false;		//set to true when a combo's been completed
	clock_t now = clock();
	int whichCombo = -1;

	//if the time window is closed or the buffer is full
	if(comboStartTime != 0 && now - comboStartTime > COMBO_TIME || cbIter >= 3)
	{
		//reset everything
		cbIter = 0;
		for(int i = 0; i < 4; i++)
			comboBuff[i] = '\0';
		//it won't come back in here unless a combo has been started
		comboStartTime = 0;
	}
	else
	{
		comboBuff[cbIter] = input;
		cbIter++;
		//if iterator is equal to or greater than the min combo size (2)
		if(cbIter >= 2)
		{
			//step through each combo sequence
			for(int j = 0; j < NUM_COMBOS; j++)
			{
				//only go to the next combo if the previous one wasn't right
				if(comboFlag == false)
				{
					//have to check the first char in the buffer seperately
					if(comboBuff[0] == comboDefinitions[j][0])//&& comboDefinitions[j][0] != '\0')
					{
						comboFlag = true;
						whichCombo = j;
						comboStartTime = now;
					}
					//step through each key of the combo sequence
					for(int i = 1; i < 4; i++)
					{
						//makes sure that each char is right not just the last one
						if(comboFlag == true)
						{
							//the != '\0' makes sure that any '\0' char is not checked
							// if the keys match remeber which combo and set combo to true
							if(comboBuff[i] == comboDefinitions[j][i])// && comboDefinitions[j][i] != '\0')
							{
								comboFlag = true;
								whichCombo = j;
							}
							//if keys don't match set combo to false
							else //if(comboDefinitions[j][i] != '\0')
								comboFlag = false;
						}
					}
				}
			}
		}
	}

	//if one of the combos was a full match
	if(comboFlag == true)
	{
		switch(whichCombo)
		{
		case 0:
			for(int i = 0; i < 4; i++)
			comboBuff[i] = '\0';
			cbIter = 0;
			return '1';
		case 1:
			for(int i = 0; i < 4; i++)
			comboBuff[i] = '\0';
			cbIter = 0;
			return 'R';
		case 2:
			for(int i = 0; i < 4; i++)
			comboBuff[i] = '\0';
			cbIter = 0;
			return 'L';
		default:
			for(int i = 0; i < 4; i++)
			comboBuff[i] = '\0';
			cbIter = 0;
			return 'i';
		}
	}
	else
		return 'i';
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