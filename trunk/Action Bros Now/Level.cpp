#include "Level.h"

Level::Level()
{
	progress = 0;
}

Level::~Level()
{
	tiles.clear();
}

bool Level::loadTiles()
{
	std::ifstream fin;
	int i = 0; int j = 0;
	int lvl = progress/3; int sublvl = progress%3;
	char check = ' ';
	char fname[MAXCHARSIZE];
	Tile tempTile;

	//tempTile src rect will always be the same unless its moving off screen
	tempTile.src.left = 0;
	tempTile.src.right = tempTile.w;
	tempTile.src.top = 0;
	tempTile.src.bottom = tempTile.h;

	//*****apply position and textures to our tiles*****
	sprintf_s(fname, MAXCHARSIZE, "./lvlSprites/lvl%isprites/render%i-%i.txt", lvl, lvl, sublvl);
	fin.open(fname);
	if(!fin.is_open())
		return false;

/*	READ IN .TXT ASCII TILES AND POSITION		*/
	fin >> check;
	while(!fin.eof())
	{
		if(check == '#')
		{
			++j;
			i = 0;
		}
		else
		{
			tempTile.key = check;
			if(tempTile.key == '.')
			{
				tempTile.ptr = NULL;
			}
			else
			{
			//set tile's ptr its corresponding texture in the container
				for(unsigned int k = 0; k < spriteContainer::getInstance()->size(); ++k)
				{
					if(spriteContainer::getInstance()->getElem(k)->key == tempTile.key)
						tempTile.ptr = spriteContainer::getInstance()->getElem(k);
				}
			}

			//do x,y calculation here: i think: if i < 30 -25 else +25; if j < 10 -25 else +25
			tempTile.pos.x = -1475.0f + i * tempTile.w;
			tempTile.pos.y = 475.0f - j * tempTile.h;
			tempTile.pos.z = 0.0f;
			tiles.push_back(tempTile);
			++i;
		}
		fin >> check;
	}
	i = j = 0;
	
	fin.close();
	return true;
}