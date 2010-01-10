#ifndef COMMON_H
#define COMMON_H

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

#define COMBO_TIME 1000
#define NUM_COMBOS		2		//number of different combos in comboDefintion
#define COMBO_HITS		3		//number of hits it takes to complete a combo
/////////////////////////////
//end input manager defines//
/////////////////////////////

#include <d3d9.h>
#include <d3dx9tex.h>

#define MAXCHARSIZE			50
///////////DEBUGMODE////////////
//   Used for Bug Testing	  //
//When enabled CPU clock speed//
//and player hitboxes will be //
//		   displayed		  //
////////////////////////////////
#define DEBUGMODE   1

#define SCREEN_WIDTH			640
#define SCREEN_HEIGHT			480

//Player Macros for player stat screen position//
//Health
#define HEALTHBAR_POSX 10
#define HEALTHBAR_POSY 10
#define HEALTHBAR_POSZ 0
#define HEALTH_POSX 35
#define HEALTH_POSY 10
#define HEALTH_POSZ 0.35f
//Special
#define SPECIALBAR_POSX 10 
#define	SPECIALBAR_POSY 35
#define SPECIALBAR_POSZ 0
#define SPECIAL_POSX 35
#define SPECIAL_POSY 35
#define SPECIAL_POSZ 0.35f

//Enemy Macros for enemy stat Screen Position//
//Health
#define ENEMY_HEALTHBAR_POSX SCREEN_WIDTH-210
#define ENEMY_HEALTHBAR_POSY 10
#define ENEMY_HEALTHBAR_POSZ 0
#define ENEMY_HEALTH_POSX SCREEN_WIDTH-210
#define ENEMY_HEALTH_POSY 10
#define ENEMY_HEALTH_POSZ 0.35f

//Boss Macros for boss stat screen position//
//Health
#define BOSS_HEALTHBAR_POSX 0
#define BOSS_HEALTHBAR_POSY SCREEN_HEIGHT-35
#define BOSS_HEALTHBAR_POSZ 0
#define BOSS_HEALTH_POSX 3 
#define BOSS_HEALTH_POSY SCREEN_HEIGHT-35
#define BOSS_HEALTH_POSZ 0.35f

//Special
#define BOSS_SPECIAL_POSX (SCREEN_WIDTH/2)+24
#define BOSS_SPECIAL_POSY SCREEN_HEIGHT-34
#define BOSS_SPECIAL_POSZ 0.30f

struct spriteSheet
{
	char key;
	char sheetName[MAXCHARSIZE];
	LPDIRECT3DTEXTURE9 gTexture;
};
struct EntitiesOnScreen
{
	int column;
	int row;
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

//Different Entity Types
enum EntityTypes
{
	PLAYER, ENEMY, BOSS
};

//for the player states
enum PlayerStates
{
	IDLE, WALK, PUNCH, KICK, STUN, COMBO1, ATTACK
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