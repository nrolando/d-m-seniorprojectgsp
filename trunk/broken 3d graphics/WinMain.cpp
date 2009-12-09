/* 
possible bugs:
*/
#include "Game.h"

#define FPS			60
#define FPSdelay	(1000/FPS)

//GLOBALS
Game *game;
HWND wndHandle;					// global window handle

bool initWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	clock_t then = clock();
	clock_t now;
	game = new Game();

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
	game->setProg(0);
	//load that level
	if(!game->loadLvl())
	{
		MessageBox(NULL, "Unable to load lvl", "ERROR", MB_OK);
		return 1;
	}

	MSG msg;
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
			game->beginRender();
			now = clock();
			game->display_time((now-then), 20);
			then = now;	//moved here for display_time()
			game->drawLvlVB();
			game->endRender();
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
	D3DXVECTOR3 camPos;
	camPos = game->getCamPos();

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case 'Q':	//****MUST BE CAPITAL OR IT WON'T READ IT...*****
			PostQuitMessage(0);
			break;
		case 'A':
			game->transCam(D3DXVECTOR3(-100.0f, 0.0f, 0.0f));
			game->pointAndSetCam(D3DXVECTOR3(camPos.x-100.0f, camPos.y, 0.0f));
			break;
		case 'S':
			game->transCam(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
			game->pointAndSetCam(D3DXVECTOR3(camPos.x, camPos.y-100.0f, 0.0f));
			break;
		case 'D':
			game->transCam(D3DXVECTOR3(100.0f, 0.0f, 0.0f));
			game->pointAndSetCam(D3DXVECTOR3(camPos.x+100.0f, camPos.y, 0.0f));
			break;
		case 'W':
			game->transCam(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
			game->pointAndSetCam(D3DXVECTOR3(camPos.x, camPos.y+100.0f, 0.0f));
			break;
		case 'R':
			game->transCam(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
			game->pointAndSetCam(D3DXVECTOR3(camPos.x, camPos.y, 0.0f));
			break;
		case 'E':
			game->transCam(D3DXVECTOR3(0.0f, 0.0f, -100.0f));
			game->pointAndSetCam(D3DXVECTOR3(camPos.x, camPos.y, 0.0f));
			break;
		};
		break;
	};

	return DefWindowProc(hWnd, message, wParam, lParam);
}