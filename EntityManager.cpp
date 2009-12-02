#include "entityManager.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

//pretty rudimentary
//file format is assumed as:
//t 2 34 (type x y)
bool EntityManager::loadFromFile(int prog)
{
	std::ifstream inFile;
	//const int maxcharsize = 50;
	int i = 0; int j = 0;
	int lvl = prog/3;
	int sublvl = prog%3;
	char fname[maxcharsize];
	char map_key = ' ';
	char check = ' ';
	int id = 0;

	while(i < maxcharsize)	//set all fname = \0
	{
		fname[i] = NULL;
		++i;
	}
	i = 0;

	sprintf_s(fname, (size_t)maxcharsize, "./lvl%isprites/loadmobs%i-%i.txt", lvl, lvl, sublvl);

	inFile.open(fname);
	if(!inFile.is_open())
		return false;

	while(i < maxcharsize)	//reset fname
	{
		fname[i] = NULL;
		++i;
	}
	i = 0;

	while(!inFile.eof())
	{
		//load texture file name into c-string
		inFile.get(check);
		while(j < maxcharsize && check != '#')
		{
			fname[j] = check;
			inFile.get(check);
			++j;
		}
		fname[j] = '\0';	//null ends the c-string
		j = 0;	//reset j

		inFile >> map_key;
		inFile.ignore();
		
		if(map_key == 'b' || map_key == 'B')
			entityVector.push_back(new Boss(id, map_key, fname));
		else
			entityVector.push_back(new Enemy(id, map_key, fname));

		id++;
	}

}