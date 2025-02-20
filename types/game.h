#ifndef GAME_H
#define GAME_H

#include "../dsa/heap.h"
#include "character.h"

typedef struct event {
    int turn_time;
    character character;
} event;

typedef struct game {
    heap events;
    dungeon *maps;
    int num_maps;
} game;

#endif
