#ifndef GAME_H
#define GAME_H

#include "character.h"
#include "dungeon.h"
#include "heap.h"

typedef struct event {
  int turn_time;
  character *character;
} event;

typedef struct game {
  heap events;
  dungeon *maps;
  int num_maps;
  int current_map;
} game;

#endif
