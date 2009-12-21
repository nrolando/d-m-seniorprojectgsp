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

	std::vector<spriteSheet> spriteCont;
	std::vector<spriteSheet> entityCont;

public:
	//singleton stuff
	static spriteContainer* getInstance();

	//access the lvl sprite vector
	bool isEmpty()					{ return spriteCont.empty(); }
	void clearVec()					{ spriteCont.clear(); }
	void push(spriteSheet ss)		{ spriteCont.push_back(ss); }
	size_t size()					{ return spriteCont.size(); }
	spriteSheet* getElem(int i)		{ return &spriteCont[i]; }

	//access the entity container (EC)
	bool EC_isEmpty()					{ return entityCont.empty(); }
	void EC_clearVec()					{ entityCont.clear(); }
	void EC_push(spriteSheet SS)		{ entityCont.push_back(SS); }
	size_t EC_size()					{ return entityCont.size(); }
	spriteSheet* EC_getElem(int i)		{ return &entityCont[i]; }
};

#endif