#ifndef COMMON_H
#define COMMON_H

#define MAXCHARSIZE			50

#include <d3d9.h>
#include <d3dx9tex.h>
#include <d3dx9.h>
#include <string>

//a struct for the player's data
struct eSprInfo
{
		D3DXVECTOR3 POS;
		LPD3DXSPRITE gSprite;
		RECT drawRect, cBox,
			 threatBox, hitBox;
};

//this is the struct for th sprite container!!!
struct spriteSheet
{
	std::string sheetName;
	LPD3DXSPRITE gSprite;
};

// the sprite container
struct Sprite
{
	char filename[MAXCHARSIZE];
	char s;							//char to represent sprite
	IDirect3DSurface9 *spriteSurf;
	int width, height;
};

//individual sprites rendered to screen, used in drawlvlvb(). store this shit so it you dont have to cycle through the sprites
struct SpriteRend
{
	float x;
	float y;
	Sprite *ptr;	//a pointer to a sprite
};


//for the player states
enum PlayerStates
{
	IDLE, WALK, ATTACK, SPECIAL, STUN,
};

#endif