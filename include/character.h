#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"

#define PLAYER_TRAIT 16

typedef struct character {
  int id;
  point pos;
  int traits;
  int speed;
  char sprite;
  int dist_to_player[DUNGEON_HEIGHT][DUNGEON_WIDTH];
  int alive;
} character;

#endif
