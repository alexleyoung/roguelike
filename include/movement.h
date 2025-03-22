#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdio.h>
#include <stdlib.h>

#include <character.h>
#include <dungeon.h>
#include <pathfinding.h>
#include <ui.h>
#include <utils.h>

enum {
  PLAYER_MOVE_SUCCESS,
  PLAYER_MOVE_QUIT,
  PLAYER_MOVE_MENU,
  PLAYER_MOVE_STAIR,
};

int move_player(dungeon *d, character *c, int move);
int move_character(dungeon *d, character *c);

#endif
