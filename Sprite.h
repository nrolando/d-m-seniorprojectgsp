#ifndef SPRITE_H
#define SPRITE_H

#include <d3d9.h>

#define MAXCHARSIZE			50

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

#endif