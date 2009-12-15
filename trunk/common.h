#ifndef COMMON_H
#define COMMON_H

#define MAXCHARSIZE			50
#define SPRITE_HEIGHT 128
#define SPRITE_WIDTH 128

#include <d3d9.h>
#include <d3dx9tex.h>
#include <d3dx9.h>
#include <string>

//this is the struct for th sprite container!!!
//D3DXCreateSprite(dxMgr->getpd3dDevice(),&gSprite);
//Load a cavemen texture
//D3DXCreateTextureFromFileEx(dxMgr->getpd3dDevice(),"gekido2.png",D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,D3DX_DEFAULT, 0,D3DFMT_UNKNOWN,
//								D3DPOOL_DEFAULT, D3DX_FILTER_NONE,D3DX_FILTER_NONE,0xFFFFFFFF,NULL,NULL,&gTexture);
struct spriteSheet
{
	std::string sheetName;
	LPD3DXSPRITE gSprite;
	LPDIRECT3DTEXTURE9 gTexture;
};


//a struct for the player's data
struct eSprInfo
{
		D3DXVECTOR3 POS;
		spriteSheet *spriteSheet;
		RECT drawRect, cBox,
			 threatBox, hitBox;
		int framenumber;
};

//for the player states
enum PlayerStates
{
	IDLE, WALK, ATTACK, SPECIAL, STUN,
};

#endif