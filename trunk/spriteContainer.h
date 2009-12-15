#ifndef SPRITECONTAINER_H
#define SPRITECONTAINER_H

#include <d3d9.h>
#include <vector>
#include <fstream>
#include <string>
#include "common.h"

#define MAXCHARSIZE			50

class spriteContainer
{
private:
	//singleton stuff
	static spriteContainer* instance;
	spriteContainer();

	std::vector<spriteSheet> spriteCont;
	
	LPDIRECT3DDEVICE9 pd3dDevice;

public:
	//singleton stuff
	static spriteContainer* getInstance();

	//get a device ptr
	void setDevice(LPDIRECT3DDEVICE9 device) {pd3dDevice = device;}
	//access the vector
	//vector<Sprite> getSC()			{ return spriteCont; }
	bool isEmpty()					{ return spriteCont.empty(); }
	void clearVec()					{ spriteCont.clear(); }
	//void push(Sprite sprite)		{ spriteCont.push_back(sprite); }
	size_t size()					{ return spriteCont.size(); }
	//Sprite* getElem(int i)			{ return &spriteCont[i]; }
	
	bool loadSpriteSheet();

	//returns a ptr to the struct with the corresponding name
	spriteSheet* getSheetPTR(std::string sheetName);
};

#endif