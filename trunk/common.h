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

enum PlayerStates
{
	IDLE, WALK, ATTACK, SPECIAL, STUN,
};

//use this for position stuff
struct Vector2D
{
	float x;
	float y;
};

#endif