#ifndef STATUS_H
#define STATUS_H

#include <string>

enum ID
{
	JASON,
	MARCUS,
	SHREDDER
};

inline std::string GetNameOfEntity(char n)
{
  switch(n)
  {
  case 'e':

    return "SmallEnemy";

  case 'E':
    
    return "LargeEnemy"; 

  case 'B':
    
    return "Shredder";

  default:

    return "UNKNOWN!";
  }
}

enum status_type
{
  OutOfRange,
  InRange,
  InFace,
  EnemyDead,
  WeakerThanEnemy,
  LessThanFiftyHealth,
  LessThanTwentyFiveHealth
};

#endif