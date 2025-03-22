#ifndef GAME_H
#define GAME_H

#include <character.h>
#include <dungeon.h>

#define DEFAULT_ROOM_COUNT 6
#define DEFAULT_MOB_COUNT 0

typedef struct game {
  dungeon *maps;
  int num_maps;
  int current_map;
} game;

#endif
