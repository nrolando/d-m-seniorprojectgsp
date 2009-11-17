#ifndef STATUS_H
#define STATUS_H

#include <string>

enum ID
{
	JASON,
	MARCUS,
	SHREDDER
};

inline std::string GetNameOfEntity(int n)
{
  switch(n)
  {
  case JASON:

    return "Jason";

  case MARCUS:
    
    return "Marcus"; 

  case SHREDDER:
    
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