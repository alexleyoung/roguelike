#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdint.h>

#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21

typedef struct point {
    uint8_t r, c;
} point;

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
    tile tiles[DUNGEON_HEIGHT][DUNGEON_WIDTH];

    room *rooms;
    uint16_t num_rooms;
    stair *stairs;
    uint16_t num_stairs;

    point player;

    int dists[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    int tunnel_dists[DUNGEON_HEIGHT][DUNGEON_WIDTH];
} dungeon;

#endif
