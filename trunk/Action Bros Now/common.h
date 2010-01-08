#ifndef COMMON_H
#define COMMON_H

#define MAXCHARSIZE			50
#define DEBUGMODE   0		//for bug testing *DO NOT CHANGE OR DELETE*

#include <d3d9.h>
#include <d3dx9tex.h>

struct spriteSheet
{
	char key;
	char sheetName[MAXCHARSIZE];
	LPDIRECT3DTEXTURE9 gTexture;
	LPDIRECT3DTEXTURE9 hBoxTexture;
	LPDIRECT3DTEXTURE9 tBoxTexture;
};

struct Tile
{
	D3DXVECTOR3 pos;
	static const int w = 50;
	static const int h = 50;
	RECT src;
	char key;
	spriteSheet *ptr;	//a pointer to a sprite
};

//a struct for the player's data
struct eSprInfo
{
	int width;
	int height;
	D3DXVECTOR3 POS;
	spriteSheet *ss_ptr;
	RECT drawRect, cBox,
		 threatBox, hitBox;
};

//for the player states
enum PlayerStates
{
	IDLE, WALK, PUNCH, KICK, SPECIAL, STUN, ATTACK
};

//THIS IS NICKS TEMP STATES FOR THE ENEMY SO I CAN GET HIM TO WALK, AND STUN TO TEST COLLISION
//IDRK HOW ENEMYOWNEDSTATES WORK YET
enum EnemyStates
{
	//CS = clawSoldier
	CS_WALK, CS_RUN, CS_HIT1, CS_DIE, CS_HIT2
};

#endif