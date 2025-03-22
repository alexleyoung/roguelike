#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "character.h"
#include "dungeon.h"

enum { PLAYER_MOVE_SUCCESS, PLAYER_MOVE_QUIT, PLAYER_MOVE_MENU };

int move_player(dungeon *d, character *c, int move);
int move_character(dungeon *d, character *c);

#endif
