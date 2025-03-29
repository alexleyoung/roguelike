#ifndef GAME_HPP
#define GAME_HPP

#include <character.hpp>
#include <dungeon.hpp>

#define DEFAULT_ROOM_COUNT 6
#define DEFAULT_MOB_COUNT 10

/*int RANDOMIZE_MONSTER_SPAWNS = 0;*/

typedef struct game {
  dungeon *maps;
  int num_maps;
  int current_map;
} game;

#endif
