#ifndef COMMON_H
#define COMMON_H

#define MAXCHARSIZE			50
#define DEBUGMODE   0		//for bug testing *DO NOT CHANGE OR DELETE*

/////////////////////////////
//defines for input manager//
/////////////////////////////
#define INPUT_UP	0x0001
#define INPUT_DOWN  0x0002
#define INPUT_LEFT	0x0004
#define INPUT_RIGHT 0x0008
#define INPUT_Z		0x0010
#define INPUT_X		0x0020
#define INPUT_C		0x0040

#define KEYDOWN(name, key) (name[key] & 0x80)

#define COMBO_TIME 10000
/////////////////////////////
//end input manager defines//
/////////////////////////////

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

//predefined combo inputs for the inputmanager
static char comboDefinitions[2][3] = {'p','p','p',
									  'r','r','p',};

#endif