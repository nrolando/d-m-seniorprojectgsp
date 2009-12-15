#ifndef COMMON_H
#define COMMON_H

#define MAXCHARSIZE			50

#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>

//a struct for the player's data
struct eSpriInfo
{
	D3DXVECTOR3 Pos;
	D3DXSPROTE* gSprite;
	Rect drawRect;
	Rect cBox;
	Rect hitBox;
	Rect threatBox;
};

//this is the struct for th sprite container!!!
struct spriteSheet
{
	string sheetName;
	D3DXSPRITE gSprite;
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

//use this for position stuff
struct Vector2D
{
	float x;
	float y;
};

//for the player states
enum PlayerStates
{
	IDLE, WALK, ATTACK, SPECIAL, STUN,
};

#endif