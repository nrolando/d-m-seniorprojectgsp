#include "Game.h"
#include <windows.h>

#define FPSdelay	16		//doesn't allow the game to go faster than 62 frames a second

//GLOBALS
Game* game = NULL;
HWND wndHandle;					// global window handle

bool initWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	clock_t then = clock();
	clock_t now;

	MSG msg;
 	game = new Game(hInstance, wndHandle);
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
			now = clock();
			if(!game->update((now-then)))
				return 1;
			then = now;
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
	wcex.lpszClassName	= "Fury";
	wcex.hIconSm		= 0;
	RegisterClassEx(&wcex);

	if(FULLSCREEN)
		wndHandle = CreateWindow("Fury", 
								 "Fury", 
								 WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE,
								 CW_USEDEFAULT, 
								 CW_USEDEFAULT, 
								 SCREEN_WIDTH, 
								 SCREEN_HEIGHT, 
								 NULL, 
								 NULL, 
								 hInstance, 
								 NULL);
	else
		wndHandle = CreateWindow("Fury", 
								 "Fury", 
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
	case WM_KEYDOWN:
		switch(wParam)
		{
		case 'Q':
			PostQuitMessage(0);
			break;
		};
		break;
	case WM_ACTIVATE:
		game->activateInput();
		switch(wParam)
		{
		case WA_INACTIVE:
			game->deavtivateInput();
			break;
		case WA_ACTIVE:
			if(!game == NULL)
				game->activateInput();
			break;
		};
	
	};

	return DefWindowProc(hWnd, message, wParam, lParam);
}