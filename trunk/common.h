#ifndef COMMON_H
#define COMMON_H

#define MAXCHARSIZE			50

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
Struct SpriteSheet
{
	char fileName[MAXCHARSIZE];
	char id;
	IDirect3DSurface9 *spriteSurf;
}


#endif