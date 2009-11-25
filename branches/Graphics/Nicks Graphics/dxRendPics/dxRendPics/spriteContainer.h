#ifndef SPRITECONTAINER_H
#define SPRITECONTAINER_H

class spriteContainer
{
private:
	//singleton stuff
	static spriteContainer* instance;
	spriteContainer();

	//put other stuf here

public:
	//singleton stuff
	static spriteContainer* getInstance();
};