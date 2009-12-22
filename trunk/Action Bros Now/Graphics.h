#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <d3d9.h>
#include <d3dx9tex.h>
#include <windows.h>
#include <string>
#include "spriteContainer.h"
#include "Boss.h"
#include <fstream>
#include <ctime>
#include <vector>

//couldn't render textures using the D3DFVF_DIFFUSE flag.
#define DIRECTINPUT_VERSION		0x0800
#define SCREEN_WIDTH			640
#define SCREEN_HEIGHT			480
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)
#define MAXSPRITESPERSUBLVL		50

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

//SPRITE STUFF***************{
	LPD3DXSPRITE	        gSprite;
//*********************************}

	//direct text
	ID3DXFont *m_font;

//the pixel rect that is grabbed from the level is based on this simulated
//camera's position
	D3DXVECTOR3 camPos;

public:
	Graphics();
	~Graphics();

	bool initD3D(HWND&);
	void _shutdown();

	void BeginRender();	//clears screen, begin scene
	void EndRender();	//end scene and present

	LPDIRECT3DTEXTURE9 getTextureFromBitmap(char*);
	LPDIRECT3DTEXTURE9 getTextureFromBitmap(char*, int&, int&);

//loads the player's current level//will be used at start of main and in update()
	//bool loadLvlFromFile(int);
	void drawLvl(std::vector<BaseGameEntity*>, eSprInfo, std::vector<Tile>, int);		//draw lvl surfaces
	//load the sprite containers
	bool loadEntityCont();
	bool loadSpriteCont(int);

	void displayTime(clock_t, int);

	//camera functions
	void moveCamera(D3DXVECTOR3);	
	void setCamPos(D3DXVECTOR3 p)	{ camPos = p; }
	D3DXVECTOR3 getCameraPos()		{ return camPos; }
	void updateCamera(eSprInfo);
};

#endif