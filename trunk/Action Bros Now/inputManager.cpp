//#include "inputManager.h"
//
//inputManager* inputManager::instance = 0;
//
//inputManager* inputManager::getInstance()
//{
//	if(instance == 0)
//		instance = new inputManager();
//	return
//		instance;
//}
//
//inputManager::inputManager()
//{
//	Carray[0] = 'u';
//	Carray[1] = 'd';
//	Carray[2] = 'u';
//
//	combo = false;
//	timesUP = false;
//
//	curTime = prevTime = 0;
//}
//
//bool inputManager::isEmpty()
//{
//	if(inputBuffer.empty())
//		return true; 
//	else 
//		return false;
//}
//
//void inputManager::setInput(WPARAM wParam, clock_t time)
//{
//	curTime = time;
//
//	if(wParam == VK_UP)//if(GetAsyncKeyState(VK_UP))
//	{
//		inputBuffer.push('u');
//	}
//	else if(wParam == VK_DOWN)//else if(GetAsyncKeyState(VK_DOWN))
//	{
//		inputBuffer.push('d');
//	}
//
//	if(prevTime != 0 && (curTime - prevTime) < COMBO_TIME)
//	{
//		timesUP = true;
//		prevTime = 0;
//	}
//	else
//	{
//		timesUP = false;
//		prevTime = curTime;
//	}
//}
//
//char inputManager::eval_ret()
//{
//	int check = 0;
//	
//	if(inputBuffer.size() >= 3)
//	{
//		char *temp = &inputBuffer.front();
//		for(int i = 0; i < 3; i++)
//		{
//			if(*temp == Carray[i])
//				check++;
//			temp += sizeof(char);
//		}
//
//		if(check == 3)
//			combo = true;
//			
//	}
//
//	if(combo == true)
//	{
//		for(int i = 0; i < 3; i++)
//		{
//			inputBuffer.pop();
//		}
//		combo =  false;
//		return '1';
//	}
//	else
//	{
//		char rChar = inputBuffer.front();
//		 inputBuffer.pop();
//		 return rChar;
//	}
//}