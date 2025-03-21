#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "character.h"
#include "dungeon.h"

/*enum {*/
/*  SUCCESS,*/
/*  PLAYER_KILLED,*/
/*};*/

int move_player(dungeon *d, character *c, int move);
int move_character(dungeon *d, character *c);

#endif
