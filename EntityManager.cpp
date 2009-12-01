#include "entityManager.h"

EntityManager* EntityManager::instance = 0;

EntityManager::EntityManager()
{
}

EntityManager* EntityManager::getInstance()
{
	if(instance == 0)
		instance = new EntityManager();
	
	return 
		instance;
}

//pretty rudimentary
//file format is assumed as:
//t 2 34 (type x y)
void EntityManager::loadFromFile(char *fileName)
{
	char enType;
	Vector2D enPOS;

	std::ifstream inFile;
	inFile.open(fileName);

	while(!inFile.eof())
	{
		if(inFile.get() != '\n')
		{
			/*inFile.get(enType);
			inFile.get(enPOS.x);
			inFile.get(enPOS.y);*/
		}
	}

}