#ifndef COMMON_H
#define COMMON_H

#define MAXCHARSIZE			50

#include <d3d9.h>
#include <d3dx9tex.h>

//use this for position stuff
struct Vector2D
{
	float x;
	float y;
};

//use this for boxes
struct Rectangle
{
	float top;
	float left;
	float width;
	float height;
};

//use this in the Sprite Container
struct SpriteSheet
{
	char fileName[MAXCHARSIZE];
	char id;
	IDirect3DSurface9 *spriteSurf;
};


#endif