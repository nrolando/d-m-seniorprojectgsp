#include "Enemy.h"
#include "Boss.h"
#include "Status.h"
#include <conio.h>
#include <windows.h>

int main()
{
	Enemy enemy(JASON,GetNameOfEntity(JASON));
	enemy.ChangeStatus(InFace);
	while(1)
	{
		enemy.UpdateState();
		if(_kbhit())
		{
			int x = _getch();
			
			//i player in Range
			if(x == 105)
				enemy.ChangeStatus(InRange);
			//o player out of range
			else if(x == 111)
				enemy.ChangeStatus(OutOfRange);
			//p player dead
			else if(x == 112)
				enemy.ChangeStatus(EnemyDead);
			//esc
			else if(x == 27)
				break;
		}
		Sleep(800);
	}
	return 0;
}