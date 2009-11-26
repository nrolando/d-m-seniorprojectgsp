#ifndef SPRITECONTAINER_H
#define SPRITECONTAINER_H

#include <d3d9.h>

#define MAXCHARSIZE			50

class spriteContainer
{
private:
	//singleton stuff
	static spriteContainer* instance;
	spriteContainer();

	//put other stuf here
	char filename[MAXCHARSIZE];
	char s;							//char to represent sprite
	IDirect3DSurface9 *spriteSurf;
	int width, height;

public:
	//singleton stuff
	static spriteContainer* getInstance();
};

#endif