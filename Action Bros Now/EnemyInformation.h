

/**ENEMY STATES**/
enum ENEMY_STATES
{
	E_IDLE, E_WALK, E_RUN, E_ATTACK1, E_ATTACK2, E_STUN, E_FALL
};

/**ENEMY TYPES (defined in ./enemySprites/load.txt)**/
#define SOLDIER1				'1'
#define SOLDIER1STATES			 7

#define SOLDIER1_AVOID_RANGE	80



/*BOSSES*/

//Soldier Boss Information//
//General
#define GENERALANIMATION	80
#define SBIDLEANIMATION	    80
#define	SBWALKANIMATION     40
#define SBRUNANIMATION		40
#define SBKICKANIMATION		40
#define SBSLASHANIMATION    60

#define SB_IDLE_FRAME		4
#define SB_WALK_FRAME		12
#define SB_RUN_FRAME		12
#define SB_KICK_FRAME		8
#define SB_SLASH_FRAME		10
#define SB_DEATH_FRAME		8
#define SB_TAUNT_FRAME		10

#define SB_RUN_SPEED		3.0f
#define SB_WALK_SPEED		1.0f

//State and combat
#define SOLDIER_BOSS	'2'
#define SBTOTALSTATES	 8

#define SB_ATTACK_RANGE  80
#define SB_RUSH_RANGE	 500
#define SB_DEFENSE_RANGE 100
#define SB_RANGE_OFFSET  50

enum SOLDIER_BOSS_STATES
{
	SB_IDLE, SB_WALK, SB_RUN, SB_KICK, SB_SLASH, SB_STUN, SB_DIE, SB_TAUNT
};