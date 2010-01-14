#include "Level.h"

Level::Level()
{
	progress = 0;
}

Level::~Level()
{
	backTiles.clear();
	frontTiles.clear();
}

bool Level::loadTiles()
{
	std::ifstream fin, inFile;
	int i = 0; int j = 0;
	int lvl = progress/3; int sublvl = progress%3;
	char check = ' ';
	char fname[MAXCHARSIZE];
	Tile tempTile;

	if(!backTiles.empty())
		backTiles.clear();
	if(!frontTiles.empty())
		frontTiles.clear();

	//tempTile src rect will always be the same unless its moving off screen
	tempTile.src.left = 0;
	tempTile.src.right = tempTile.w;
	tempTile.src.top = 0;
	tempTile.src.bottom = tempTile.h;

	//*****apply position and textures to our tiles*****
	sprintf_s(fname, MAXCHARSIZE, "./lvlSprites/lvl%isprites/render%i-%iback.txt", lvl, lvl, sublvl);
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

			//apply position: x,y,z is the upper left corner of image. background is always 3000x1000
			tempTile.pos.x = -1500.0f + i * tempTile.w;
			tempTile.pos.y = 500.0f - j * tempTile.h;
			tempTile.pos.z = 0.0f;
			backTiles.push_back(tempTile);
			++i;
		}
		fin >> check;
	}
	i = j = 0;
	fin.close();

//GET FOREGROUND TILES
	//*****apply position and textures to our tiles*****
	sprintf_s(fname, MAXCHARSIZE, "./lvlSprites/lvl%isprites/render%i-%ifront.txt", lvl, lvl, sublvl);
	inFile.open(fname);
	if(!inFile.is_open())
		return false;

/*	READ IN .TXT ASCII TILES AND POSITION		*/
	inFile >> check;
	while(!inFile.eof())
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

			//apply position: x,y,z is the upper left corner of image. background is always 3000x1000
			tempTile.pos.x = -1500.0f + i * tempTile.w;
			tempTile.pos.y = 500.0f - j * tempTile.h;
			tempTile.pos.z = 0.0f;
			frontTiles.push_back(tempTile);
			++i;
		}
		inFile >> check;
	}
	i = j = 0;
	inFile.close();

	return true;
}