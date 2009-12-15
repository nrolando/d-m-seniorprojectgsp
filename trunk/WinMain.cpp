/* 
possible bugs: is it just me, or is the game really really laggy
*/
#include "Game.h"
#include "inputManager.h"
#include "inputManager2.h"
#include <windows.h>

//GLOBALS
Game* game;
HWND wndHandle;					// global window handle

bool initWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	clock_t then = clock();
	clock_t now;
	game = new Game(clock(), hInstance, wndHandle);
	MSG msg;

	// call our function to init and create our window
	if (!initWindow(hInstance))
	{
		MessageBox(NULL, "Unable to create window", "ERROR", MB_OK);
		return 1;
	}
	if(!game->initGame(wndHandle))
	{
		MessageBox(NULL, "Unable to initialize Direct3D", "ERROR", MB_OK);
		return 1;
	}

	//get player's progress from load file or new game (0)
	//game->setProg(0);
	//load that level
	if(!game->loadSprites())
	{
		MessageBox(NULL, "Unable to load lvl", "ERROR", MB_OK);
		return 1;
	}
	
	game->setSheetPtr();
	
    ZeroMemory( &msg, sizeof(msg) );
	while(msg.message!=WM_QUIT)
    {
		// check for messages
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
			TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		else
		{
			
			game->gameUpdate();
			
			//now = clock();
			//game->display_time((now-then), 20);
			//then = now;	//moved here for display_time()
			// pause to force frames per second
			//while(clock() < now + FPSdelay)
			//	Sleep(1);
		}
	}
	game->_shutdown();
	delete game;
	return 0;
}

bool initWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "DirectXExample";
	wcex.hIconSm		= 0;
	RegisterClassEx(&wcex);

	wndHandle = CreateWindow("DirectXExample", 
							 "DirectXExample", 
							 WS_OVERLAPPEDWINDOW,
							 CW_USEDEFAULT, 
							 CW_USEDEFAULT, 
							 SCREEN_WIDTH, 
							 SCREEN_HEIGHT, 
							 NULL, 
							 NULL, 
							 hInstance, 
							 NULL);
   if (!wndHandle)
      return false;
   
   ShowWindow(wndHandle, SW_SHOW);
   UpdateWindow(wndHandle);

   return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	};

	return DefWindowProc(hWnd, message, wParam, lParam);
}