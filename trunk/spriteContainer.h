#ifndef SPRITECONTAINER_H
#define SPRITECONTAINER_H

#include <d3d9.h>
#include <vector>
#include "common.h"

#define MAXCHARSIZE			50

class spriteContainer
{
private:
	//singleton stuff
	static spriteContainer* instance;
	spriteContainer();

	std::vector<Sprite> spriteCont;
	std::vector<spriteSheet> ss;

public:
	//singleton stuff
	static spriteContainer* getInstance();

	//access the vector
	//vector<Sprite> getSC()			{ return spriteCont; }
	bool isEmpty()					{ return spriteCont.empty(); }
	void clearVec()					{ spriteCont.clear(); }
	void push(Sprite sprite)		{ spriteCont.push_back(sprite); }
	size_t size()					{ return spriteCont.size(); }
	Sprite* getElem(int i)			{ return &spriteCont[i]; }

	//access the spriteSheet
	bool SS_isEmpty()					{ return ss.empty(); }
	void SS_clearVec()					{ ss.clear(); }
	void SS_push(spriteSheet SS)		{ ss.push_back(SS); }
	size_t SS_size()					{ return ss.size(); }
	spriteSheet* SS_getElem(int i)		{ return &ss[i]; }
	
};

#endif