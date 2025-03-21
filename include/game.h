#ifndef GAME_H
#define GAME_H

#include "../dsa/heap.h"
#include "../dsa/j_heap.h"
#include "dungeon.h"
#include "character.h"

typedef struct event {
    int turn_time;
    character *character;
} event;

typedef struct game {
    heap events;
    /*j_heap_t events;*/
    dungeon *maps;
    int num_maps;
    int current_map;
} game;

#endif
