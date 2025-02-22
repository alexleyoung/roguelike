#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdint.h>

#include "types.h"
#include "character.h"

typedef struct tile {
    char sprite;
    int hardness;
} tile;

typedef struct stair {
    point p;
    int type;
} stair;

typedef struct room {
    point corner;
    point size;
} room;

typedef struct dungeon {
    int id;

    tile tiles[DUNGEON_HEIGHT][DUNGEON_WIDTH];

    room *rooms;
    uint16_t num_rooms;
    stair *stairs;
    uint16_t num_stairs;

    int dists[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    int tunnel_dists[DUNGEON_HEIGHT][DUNGEON_WIDTH];

    point player_pos;
    character *character_map[DUNGEON_HEIGHT][DUNGEON_WIDTH];
} dungeon;

#endif
