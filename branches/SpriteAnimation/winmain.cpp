#include <windows.h>
#include <dinput.h>
#include <time.h>

#include "dxManager.h"

HINSTANCE hInst;				// holds the instance for this app
HWND wndHandle;					// global window handle
clock_t start_time,new_time;

//Directx Sprtie input
LPDIRECTINPUT8        g_lpDI;	    // the direct input object
LPDIRECTINPUTDEVICE8  g_lpDIDevice; // the direct input device

// DX global interfaces
LPD3DXSPRITE	        gSprite;	// the D3DX sprite interfaces
LPDIRECT3DTEXTURE9      gTexture;	// the main animation texture

// timer stuff
LARGE_INTEGER timeStart;
LARGE_INTEGER timeEnd;
LARGE_INTEGER timerFreq;
float         anim_rate;

#define SPRITE_WIDTH 128	
#define SPRITE_HEIGHT 128
#define PNG_WIDTH 1952
#define PNG_HEIGHT 488
#define WAIT_TIME 60

//definition of input flags
#define INPUT_UP    0x0001
#define INPUT_DOWN  0x0002
#define INPUT_LEFT  0x0004
#define INPUT_RIGHT 0x0008

char buffer[256]; 
int  inputflag = 0;
int  lastinput = 0;
int  downflag = 0;

#define KEYDOWN(name, key) (name[key] & 0x80) 

enum { LEFT_ARROW = 0,
		UP_ARROW,
		DOWN_ARROW,
		RIGHT_ARROW };


// this holds the DirectX Manager
dxManager *dxMgr;
// this is the surface that will hold the sprites
IDirect3DSurface9* spriteSurface;

#define MAX_SPRITES 1			// this is the number of sprites we want
#define MAX_LOCX 640		// don't want the fish to go off the screen
#define MAX_LOCY 480

// this is the sprite structure
struct {
	RECT srcRect;				// holds the location of this sprite in the source bitmap

	D3DXVECTOR3 pos;
	D3DXVECTOR3 cPos;

	// movement
	float moveX;
	float moveY;

	// animation
	int numFrames;				// number of frames this animation has
	float curFrame;				// the current frame of animation
} spriteStruct;

//RECT destRect;

////////////////////////////////////////////// forward declarations
bool    initWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// sprite functions
bool initSprites(void);
void drawSprite(WPARAM);
void setinput();
void inputcheck();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HRESULT hr;

	// call our function to init and create our window
	if (!initWindow(hInstance))
	{
		MessageBox(NULL, "Unable to create window", "ERROR", MB_OK);
		return false;
	}

	// create the directx manager
	dxMgr = new dxManager();

	// Create the DirectInput object. 
    hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, 
                            IID_IDirectInput8, (void**)&g_lpDI, NULL); 

	if FAILED(hr) return FALSE; 

    // Retrieve a pointer to an IDirectInputDevice8 interface 
    hr = g_lpDI->CreateDevice(GUID_SysKeyboard, &g_lpDIDevice, NULL); 

	hr = g_lpDIDevice->SetDataFormat(&c_dfDIKeyboard); 

	if FAILED(hr) { 
		return FALSE; 
	} 

	// Set the cooperative level 
    hr = g_lpDIDevice->SetCooperativeLevel(wndHandle, 
                             DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
    if FAILED(hr) 
    { 
        return FALSE; 
    } 

    // Get access to the input device. 
    hr = g_lpDIDevice->Acquire(); 
    if FAILED(hr) 
    { 
        return FALSE; 
    } 

	// init the directx manager
	if (!dxMgr->init(wndHandle))
	{
		MessageBox(NULL, "Unable to Init the DirectX Manager", "ERROR", MB_OK);
		return false;
	}

	// initialise the sprites
	if (!initSprites())
	{
		MessageBox(NULL, "Unable to Init the Sprites", "ERROR", MB_OK);
		return false;
	}

	// Main message loop:
	// Enter the message loop
    MSG msg; 
    ZeroMemory( &msg, sizeof(msg) );
    while( msg.message!=WM_QUIT )
    {
		new_time = clock();
		// check for messages
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
			TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		// this is called when no messages are pending
		else
		{
			hr = g_lpDIDevice->GetDeviceState(sizeof(buffer),(LPVOID)&buffer); 
			

			if((new_time - start_time) > WAIT_TIME)	
			{
				setinput();
				
				dxMgr->beginRender(gSprite);
	
				drawSprite(msg.wParam);

				dxMgr->endRender(gSprite);

				start_time = clock();
			}


			//QueryPerformanceCounter(&timeEnd);

			//anim_rate = ( (float)timeEnd.QuadPart - (float)timeStart.QuadPart ) / timerFreq.QuadPart;
		}
    }
	
	// shutdown the directX Manager
	dxMgr->shutdown();

	return (int) msg.wParam;
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
							 640, 
							 480, 
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

	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


bool initSprites(void)
{

	HRESULT hr=D3DXCreateSprite(dxMgr->getpd3dDevice(),&gSprite);
	if (FAILED(hr))
		return FALSE;

	// Load a cavemen texture
	D3DXCreateTextureFromFileEx(dxMgr->getpd3dDevice(),"gekido2.png",D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,D3DX_DEFAULT, 0,D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE,D3DX_FILTER_NONE,0xFFFFFFFF,NULL,NULL,&gTexture);
	/*D3DXCreateTextureFromFileEx (dxMgr->getpd3dDevice(), "gekido2.png", 0, 0, 1, 0, 
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
		0xFFFFFFFF, &spriteStruct.srcRect, NULL, &gTexture);*/

	//spriteSurface = dxMgr->getSurfaceFromBitmap("gekido2.png", PNG_WIDTH, PNG_HEIGHT);
	//if (spriteSurface == NULL)
	//	return false;

	spriteStruct.srcRect.top = 0;
	spriteStruct.srcRect.left = 0;
	spriteStruct.srcRect.right = spriteStruct.srcRect.left + SPRITE_WIDTH;
	spriteStruct.srcRect.bottom = spriteStruct.srcRect.top + SPRITE_HEIGHT;

	///*spriteStruct.srcRect.top = SPRITE_HEIGHT;
	//spriteStruct.srcRect.left = 0;
	//spriteStruct.srcRect.right = spriteStruct.srcRect.left + SPRITE_WIDTH;
	//spriteStruct.srcRect.bottom = spriteStruct.srcRect.top + SPRITE_HEIGHT;
	//
	//spriteStruct.srcRect.top = SPRITE_HEIGHT * 2;
	//spriteStruct.srcRect.left = 0;
	//spriteStruct.srcRect.right = spriteStruct.srcRect.left + SPRITE_WIDTH;
	//spriteStruct.srcRect.bottom = spriteStruct.srcRect.top + SPRITE_HEIGHT;

	//spriteStruct.srcRect.top = SPRITE_HEIGHT * 3;
	//spriteStruct.srcRect.left = 0;
	//spriteStruct.srcRect.right = spriteStruct.srcRect.left + SPRITE_WIDTH;
	//spriteStruct.srcRect.bottom = spriteStruct.srcRect.top + SPRITE_HEIGHT;*/

	//spriteStruct.posX = float(rand()%100);
	//spriteStruct.posY = float(rand()%300);

	//	 set the animation data
	spriteStruct.curFrame = 0;
	spriteStruct.numFrames = 7;

	//	 set the move data
	spriteStruct.moveX = 8.0;
	spriteStruct.moveY = 8.0;

	// set sprtie position
	spriteStruct.pos.x = 0.0f;
	spriteStruct.pos.y = 0.0f;
	spriteStruct.pos.z = 0.0f;

	//sprite center position
	spriteStruct.cPos.x = 0.0f;
	spriteStruct.cPos.y = 0.0f;
	spriteStruct.cPos.z = 0.0f;

	//// set the destination for this sprite
	//destRect.left = int(spriteStruct.pos.x);
	//destRect.top = int(spriteStruct.pos.y);
	//destRect.bottom = destRect.top + SPRITE_HEIGHT;
	//destRect.right = destRect.left + SPRITE_WIDTH;

	// draw the sprite to the back buffer
	//dxMgr->blitToSurface(spriteSurface, &spriteStruct.srcRect, &destRect);

	return true;
}

void drawSprite(WPARAM wParam)
{	
	inputcheck();
	gSprite->Draw(gTexture,&spriteStruct.srcRect,&spriteStruct.cPos,&spriteStruct.pos,0xFFFFFFFF);
}

void inputcheck()
{
	// move the sprite by it's movement rate
	if(inputflag & INPUT_UP)
	{
		spriteStruct.pos.y -= spriteStruct.moveY;
		if (spriteStruct.pos.y <= 0)
		spriteStruct.pos.y = 0;

		// make the sprite animation through the frames
		if (spriteStruct.curFrame < (spriteStruct.numFrames+5))
			spriteStruct.curFrame++;
		else
			spriteStruct.curFrame = 1;

		spriteStruct.srcRect.top = SPRITE_HEIGHT;
		spriteStruct.srcRect.bottom = spriteStruct.srcRect.top + SPRITE_HEIGHT;
	}
	else if(inputflag & INPUT_DOWN)
	{
		spriteStruct.pos.y += spriteStruct.moveY;
		if (spriteStruct.pos.y >= MAX_LOCY)
			spriteStruct.pos.y = MAX_LOCY;

		// make the sprite animation through the frames
		if (spriteStruct.curFrame < (spriteStruct.numFrames+5))
			spriteStruct.curFrame++;
		else
			spriteStruct.curFrame = 1;

		spriteStruct.srcRect.top = SPRITE_HEIGHT;
		spriteStruct.srcRect.bottom = spriteStruct.srcRect.top + SPRITE_HEIGHT;
	}

	else if(inputflag & INPUT_LEFT)
	{
		spriteStruct.pos.x -= spriteStruct.moveX;
		if (spriteStruct.pos.x <= 0)
			spriteStruct.pos.x = 0;

		// make the sprite animation through the frames
		if (spriteStruct.curFrame < (spriteStruct.numFrames+5))
			spriteStruct.curFrame++;
		else
			spriteStruct.curFrame = 1;

		spriteStruct.srcRect.top = SPRITE_HEIGHT;
		spriteStruct.srcRect.bottom = spriteStruct.srcRect.top + SPRITE_HEIGHT;

	}
	else if(inputflag & INPUT_RIGHT)
	{
		spriteStruct.pos.x += spriteStruct.moveX;
		if (spriteStruct.pos.x >= MAX_LOCX)
			spriteStruct.pos.x = MAX_LOCX;

		// make the sprite animation through the frames
		if (spriteStruct.curFrame < (spriteStruct.numFrames+5))
			spriteStruct.curFrame++;
		else
			spriteStruct.curFrame = 1;

		spriteStruct.srcRect.top = SPRITE_HEIGHT;
		spriteStruct.srcRect.bottom = spriteStruct.srcRect.top + SPRITE_HEIGHT;
	}
	else if(KEYDOWN(buffer,DIK_Z))
	{
		// make the sprite animation through the frames
		if (spriteStruct.curFrame < (spriteStruct.numFrames-2))
			spriteStruct.curFrame++;
		else
			spriteStruct.curFrame = 0;

		spriteStruct.srcRect.top = SPRITE_HEIGHT * 2;
		spriteStruct.srcRect.bottom = spriteStruct.srcRect.top + SPRITE_HEIGHT;

	}
	else if(KEYDOWN(buffer,DIK_X))
	{
		// make the sprite animation through the frames
		if (spriteStruct.curFrame < (spriteStruct.numFrames-2))
			spriteStruct.curFrame++;
		else
			spriteStruct.curFrame = 0;

		spriteStruct.srcRect.top = SPRITE_HEIGHT * 3;
		spriteStruct.srcRect.bottom = spriteStruct.srcRect.top + SPRITE_HEIGHT;
	}
	else
	{
		spriteStruct.srcRect.top = 0;
		spriteStruct.srcRect.bottom = spriteStruct.srcRect.top + SPRITE_HEIGHT;

		// make the sprite animation through the frames
		if (spriteStruct.curFrame < spriteStruct.numFrames-1)
		spriteStruct.curFrame++;
		else
			spriteStruct.curFrame = 0;	
	}	

	// set the source rect to the correct frame position
	spriteStruct.srcRect.left = long(spriteStruct.curFrame * SPRITE_WIDTH);
	spriteStruct.srcRect.right = spriteStruct.srcRect.left + SPRITE_WIDTH;
}

void setinput()
{
	lastinput = inputflag;

	if (KEYDOWN(buffer, DIK_LEFT))
		inputflag |= INPUT_LEFT; 
	else
		inputflag = inputflag &(~INPUT_LEFT);

	if (KEYDOWN(buffer, DIK_UP))
		inputflag |= INPUT_UP;
	else
		inputflag = inputflag &(~INPUT_UP);
	
	if (KEYDOWN(buffer, DIK_DOWN))
		inputflag |= INPUT_DOWN; 
	else
		inputflag = inputflag &(~INPUT_DOWN);

	if (KEYDOWN(buffer, DIK_RIGHT))
		inputflag |= INPUT_RIGHT;
	else
		inputflag = inputflag &(~INPUT_RIGHT);

	downflag = (inputflag^lastinput)&inputflag;
}