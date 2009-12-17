#ifndef COMMON_H
#define COMMON_H

#define MAXCHARSIZE			50

#include <d3d9.h>
#include <d3dx9tex.h>

// the sprite container
struct Sprite
{
	char filename[MAXCHARSIZE];
	char s;							//char to represent sprite
	//IDirect3DSurface9 *spriteSurf;
	LPDIRECT3DTEXTURE9 Texture;
	int width, height;
};

//individual sprites rendered to screen, used in drawlvlvb(). store this shit so it you dont have to cycle through the sprites
struct SpriteRend
{
	D3DXVECTOR3 pos;
	Sprite *ptr;	//a pointer to a sprite
};

//use this for position stuff
struct Vector2D
{
	float x;
	float y;
};

struct spriteSheet
{
	char sheetName[MAXCHARSIZE];
	LPDIRECT3DTEXTURE9 gTexture;
};


//a struct for the player's data
struct eSprInfo
{
	int width;
	int height;
	D3DXVECTOR3 POS;
	spriteSheet *ss_ptr;
	RECT drawRect, cBox,
		 threatBox, hitBox;
};

//for the player states
enum PlayerStates
{
	IDLE, WALK, ATTACK, SPECIAL, STUN,
};

#endif