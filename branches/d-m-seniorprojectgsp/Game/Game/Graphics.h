#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <d3d9.h>
#include <d3dx9tex.h>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

//couldn't render textures using the D3DFVF_DIFFUSE flag.
#define DIRECTINPUT_VERSION		0x0800
#define SCREEN_WIDTH			640
#define SCREEN_HEIGHT			480
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)
#define MAXCHARSIZE			50
#define MAXSPRITESPERSUBLVL		50

// the sprite container
struct Sprite
{
	char filename[MAXCHARSIZE];
	char s;							//char to represent sprite
	FLOAT width, height;			//size
	LPDIRECT3DTEXTURE9 g_pTexture;	// texture info
};

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
	FLOAT x, y, z;  // The untransformed, 3D position for the vertex
	float tu, tv;
};

/*a game progress variable will be used to determine current level: if prog = 0 then
level = prog/3 + 1 (+1 since the levels start at 1, not 0)
and sublevel = prog%3 + 1, so if prog = 0 then current lvl is 1-1.
so if prog = 5 then level = 2 and sublevel = 3 or 2-3.
When its time for a new level (not sublvl), a new sprite container will be made for
that whole lvls use as well as the vertex buffer for those sprites (..textures)
based on level and sublevel, the level loader function will read from that specific
.txt file that will tell us where and what sprites are to be placed 
***REFER TO CH6 EX7 FOR SAVED CODE ON DOING TRANSPARENT TEXTURES AND SETTING THEM UP*/

class Graphics
{
private:
	//direct 3d
	LPDIRECT3D9             pD3D;
	LPDIRECT3DDEVICE9       pd3dDevice;

	// vertex buffer
	LPDIRECT3DVERTEXBUFFER9 g_pVB; 

//LEVEL SPRITE STUFF***************{
	//sprite/textures for the levels
	vector<Sprite> lvlSprites;
	//number of sprites to be rendered in the lvl. needed for drawLvlVB()
	int spritesRend;
	float *px;				//position for each sprite to be rendered
	float *py;
	char *c_spr;
	////debug member
	LPDIRECT3DTEXTURE9 g_pTexture;
//*********************************}

	//camera members
	D3DXMATRIX matView;					// the view matrix
	D3DXMATRIX matProj;					// the projection matrix
	D3DXVECTOR3 cameraPosition;			// the position of the camera
	D3DXVECTOR3 cameraLook;				// where the camera is pointing
public:
	Graphics();
	~Graphics();

	bool initD3D(HWND&);
	void _shutdown();

	void BeginRender();	//clears screen, begin scene
	void EndRender();	//end scene and present

	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(int, DWORD);
	/***LOADLVL***:	
		*loads the player's current level
		*return value based on success of creating vertex buffer
		*will be used at start of main and in update()	*/
	bool loadLvlFromFile(int);
	HRESULT SetupLvlVB(int);		//vertex buffer for level
	void drawLvlVB();

	//camera functions
	void createCamera(float, float);
	void moveCamera(D3DXVECTOR3);
	void translateCamera(D3DXVECTOR3);
	void pointAndSetCamera(D3DXVECTOR3);	//para: pos, lookAt	

	D3DXVECTOR3 getCameraPos()		{ return cameraPosition; }
};

#endif