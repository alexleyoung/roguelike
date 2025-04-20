#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP

#include <stdio.h>
#include <stdlib.h>

#include <character.hpp>
#include <dungeon.hpp>
#include <pathfinding.hpp>
#include <types.hpp>
#include <ui.hpp>
#include <utils.hpp>

enum PLAYER_ACTION {
  PLAYER_MOVE,
  PLAYER_MOVE_ACTION,
  PLAYER_MOVE_INVALID,
  PLAYER_MOVE_QUIT,
  PLAYER_MOVE_MENU,
  PLAYER_MOVE_STAIR,
  PLAYER_TOGGLE_FOG,
};

int move_player(Dungeon *d, Player *c, int move);
int move_character(Dungeon *d, Character *c);
int update_player_vision(Dungeon *d, Player *p);

#endif
