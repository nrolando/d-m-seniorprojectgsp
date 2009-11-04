/* 
possible bugs:	in Graphics.loadlvlfromfile();
				might need to release the vb buffer before creating a new one
*/
#include "Graphics.h"

//GLOBALS
Graphics *g;
HWND wndHandle;					// global window handle

bool initWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	g = new Graphics();

	// call our function to init and create our window
	if (!initWindow(hInstance))
	{
		MessageBox(NULL, "Unable to create window", "ERROR", MB_OK);
		return 1;
	}
	if(!g->initD3D(wndHandle))
	{
		MessageBox(NULL, "Unable to initialize Direct3D", "ERROR", MB_OK);
		return 1;
	}

	//get player's progress from load file or new game (0)
	g->setProg(0);
	//load that level
	
	if(!g->loadLvlFromFile())
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
			g->BeginRender();
			g->drawLvlVB();
			g->EndRender();
		}
	}
	g->_shutdown();
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
	case WM_KEYDOWN:
		switch(wParam)
		{
		case 'Q':	//****MUST BE CAPITAL OR IT WON'T READ IT...*****
			PostQuitMessage(0);
			break;
		case 'A':
			g->moveCamera(D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
			break;
		case 'S':
			g->moveCamera(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
			break;
		case 'D':
			g->moveCamera(D3DXVECTOR3(1.0f, 0.0f, 0.0f));
			break;
		case 'W':
			g->moveCamera(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			break;
		};
		break;
	};

	return DefWindowProc(hWnd, message, wParam, lParam);
}