#include "spriteContainer.h"

//init static instance
spriteContainer* spriteContainer::instance = 0;

spriteContainer::spriteContainer()
{
}

spriteContainer* spriteContainer::getInstance()
{
	if(instance == 0)
		instance = new spriteContainer();

	return
		instance;
}

bool spriteContainer::loadSpriteSheet()
{
	//put the directory path to the sprites in here
	char  tempCString1[50];
	char  tempCString2[50];
	std::string tempString;

	//tempsheet
	spriteSheet tempSheet;
	//find the txt file containing the names of all the sprites
	std::ifstream inFile;
	inFile.open("./lvlsprites/loadSpr.txt");

	//cycle through the txt
	while(!inFile.eof())
	{
		//get the name of spritesheet from txt file
		inFile.getline(tempCString2, MAXCHARSIZE, '#');
		//create directory path for the spritesheet
		sprintf_s(tempCString1, MAXCHARSIZE, "./lvlsprites/%s",
			tempCString2);

		tempSheet.sheetName += tempCString2;
		
		
		D3DXCreateSprite(pd3dDevice,&tempSheet.gSprite);
		D3DXCreateTextureFromFileEx(pd3dDevice,tempCString1,
									D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
									D3DX_DEFAULT, 0,D3DFMT_UNKNOWN,
									D3DPOOL_DEFAULT, D3DX_FILTER_NONE,D3DX_FILTER_NONE,
									0xFFFFFFFF,NULL,NULL,&tempSheet.gTexture);

		spriteCont.push_back(tempSheet);
	}

	inFile.close();
	return true;
}

spriteSheet* spriteContainer::getSheetPTR(std::string sName)
{
	for(unsigned int i = 0; i <= spriteCont.size(); i++)
	{
		if(spriteCont[i].sheetName == sName)
			return &spriteCont[i];
	}
}