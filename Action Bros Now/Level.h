#ifndef LEVEL_H
#define LEVEL_H

#include "common.h"
#include "spriteContainer.h"
#include <vector>
#include <fstream>

class Level
{
private:
/*progress is incremented every sublevel, like an index. to get the level and sublevel: level = prog/3; sublevel = prog%3; */
	int progress;
	std::vector<Tile> tiles;
public:
	Level();
	~Level();

	//get methods
	int getProg()					{ return progress; }
	std::vector<Tile> getTiles()	{ return tiles; }

	//set methods
	void setProg(int p)				{ progress = p; }

	void incrementProg()			{ progress++; }
	//this loads the tile vector for the next level, should be called
	//every level/sublevel
	bool loadTiles();
};

#endif