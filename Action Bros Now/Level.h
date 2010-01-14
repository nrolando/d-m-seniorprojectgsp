#ifndef LEVEL_H
#define LEVEL_H

#include "common.h"
#include "spriteContainer.h"
#include <vector>
#include <fstream>

//remember the pos is at the upper left corner of the sprite
#define YLIMIT_TOP		25
#define YLIMIT_BOTTOM	-162

class Level
{
private:
/*progress is incremented every sublevel, like an index. to get the level and sublevel: level = prog/3; sublevel = prog%3; */
	int progress;
	std::vector<Tile> backTiles;
	std::vector<Tile> frontTiles;
public:
	Level();
	~Level();

	//get methods
	int getProg()						{ return progress; }
	std::vector<Tile> getBackTiles()	{ return backTiles; }
	std::vector<Tile> getFrontTiles()	{ return frontTiles; }

	//set methods
	void setProg(int p)				{ progress = p; }

	void incrementProg()			{ progress++; }
	//this loads the tile vector for the next level, should be called
	//every level/sublevel
	bool loadTiles();
};

#endif