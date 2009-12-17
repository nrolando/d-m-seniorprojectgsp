#ifndef SPRITE_H
#define SPRITE_H

#include <d3d9.h>
#include <d3dx9tex.h>

#define MAXCHARSIZE			50

// the sprite container
struct Sprite
{
	char filename[MAXCHARSIZE];
	char s;							//char to represent sprite
	LPDIRECT3DTEXTURE9 g_pTexture;	// texture info
};

//individual sprites rendered to screen, used in drawlvlvb(). store this shit so it you dont have to cycle through the sprites
struct SpriteRend
{
	float x;
	float y;
	Sprite *ptr;	//a pointer to a sprite
};

#endif