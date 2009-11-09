#include "Enemy.h"
#include "Status.h"
#include <windows.h>

int main()
{
	Enemy enemy(JASON);
	for(int x=0;x<20;x++)
	{
		enemy.UpdateState();
		Sleep(800);
	}
	return 0;
}