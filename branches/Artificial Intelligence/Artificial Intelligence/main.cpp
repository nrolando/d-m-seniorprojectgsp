#include "Enemy.h"
#include <windows.h>

int main()
{
	Enemy enemy;
	for(int x=0;x<20;x++)
	{
		enemy.Update();
		Sleep(800);
	}
	return 0;
}