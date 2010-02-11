#ifndef COMMON_H
#define COMMON_H

#include <d3d9.h>
#include <d3dx9tex.h>
#include <d3d9.h>
#include <d3dx9tex.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>
#include <dsound.h>
#include "dsutil.h"
#include "EnemyInformation.h"

#define FULLSCREEN		1

#define MAXCHARSIZE			50

//level boundaries
//remember the pos is at the upper left corner of the sprite
#define YLIMIT_TOP		20
#define YLIMIT_BOTTOM	-162

//////////////////
//player defines//
//////////////////

//player's attack power
#define P_POWER			10	//punch power
#define K_POWER			20	//kick power
#define C1_POWER		60	//flame hit of combo1

//////////////////////
//end player defines//
//////////////////////

/////////////////////
//animation defines//
/////////////////////

//current number of player states in spritesheet for one direction
#define NUM_STATES		10
//these are for animation and stun length
#define STUNTIME		500

//number of frames per animation
#define MAXIDLEFRAME	9
#define MAXWALKFRAME	13
#define MAXRUNFRAME		6
#define MAXPUNCHFRAME	6
#define MAXKICKFRAME	8
#define MAXSTUNFRAME	1
#define MAXCOMBO1FRAME	21
#define MAXKICK2FRAME	7
#define MAXDEATHFRAME	7
#define MAXFALLFRAME	11

//animation times
#define MAXKICKANIMATION		50
#define MAXPUNCHANIMATION		45
#define MAXCOMBO1ANIMATION		60
#define MAXDEATHANIMATION		100
#define MAXRESPAWNANIMATION		200
#define RESPAWN_WAIT_TIME		2000

/////////////////////////
//end animation defines//
/////////////////////////

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
#define INPUT_SPACE	0x0080

#define KEYDOWN(name, key) (name[key] & 0x80)

#define COMBO_TIME		600
#define NUM_COMBOS		3		//number of different combos in comboDefintion
#define COMBO_HITS		4		//number of hits it takes to complete a combo

/////////////////////////////
//end input manager defines//
/////////////////////////////


///////////DEBUGMODE////////////
//   Used for Bug Testing	  //
//When enabled CPU clock speed//
//and player hitboxes will be //
//		   displayed		  //
////////////////////////////////
#define DEBUGMODE   0

///////////////////////
//screen size defines//
///////////////////////

#define SCREEN_WIDTH		   1024
#define SCREEN_HEIGHT			768

///////////////////
//end screen size//
///////////////////

//Macros for Enemy and Bosses visual ranges
#define XRANGE_OFFSET 15.0f
#define YRANGE_OFFSET 20.0f
#define CHASE_RANGE  150
#define ATTACK_RANGE 50
#define AVOID_RANGE	 60
#define PATROL_RANGE 50

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
#define BOSS_HEALTHBAR_POSX 210
#define BOSS_HEALTHBAR_POSY SCREEN_HEIGHT-80
#define BOSS_HEALTHBAR_POSZ 0
#define BOSS_HEALTH_POSX 213 
#define BOSS_HEALTH_POSY SCREEN_HEIGHT-80
#define BOSS_HEALTH_POSZ 0.30f

//Special
#define BOSS_SPECIAL_POSX 553
#define BOSS_SPECIAL_POSY SCREEN_HEIGHT-80
#define BOSS_SPECIAL_POSZ 0.30f

/////////
//enums//
/////////

//Different Entity Types
enum EntityTypes
{
	PLAYER, ENEMY, BOSS
};

//for the title/menu screen
enum SCREENS
{
	TITLE, LOAD, OPTIONS
};

//for the player states
enum PlayerStates
{
	IDLE, WALK, PUNCH, KICK, STUN, COMBO1, RUN, KICK2, FALL, RESPAWN
};

/////////////
//end enums//
/////////////

/////////////////
//static arrays//
/////////////////

//bgm list
static char* BGMlist[7] = {"129","122","Collarbone","124","fire","movieprojector","thunder"};

//predefined combo inputs for the inputmanager
static char comboDefinitions[NUM_COMBOS][COMBO_HITS] = {'p','k','p','\0',	//uppercut combo
														'r','r','\0','\0',	//run right
														'l','l','\0','\0'};	//run left

/////////////////////
//end static arrays//
/////////////////////

///////////
//structs//
///////////

//struct for containing sounds
//bgm & sfx
struct soundFile
{
	//make this a string???
	char soundName[MAXCHARSIZE];
	LPDIRECTSOUNDBUFFER sound;
};

//struct for the sprite sheets
//player, enemies, and lvl
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
	RECT drawRect, threatBox, 
		 hitBox;
};

///////////////
//end structs//
///////////////


#endif