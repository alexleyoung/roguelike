#ifndef GEN_H
#define GEN_H

#include "types.h"

#define DEFAULT_HARDNESS 255

#define ROOM_MIN_WIDTH 4
#define ROOM_MIN_HEIGHT 3
#define ROOM_MAX_WIDTH 13
#define ROOM_MAX_HEIGHT 7

typedef struct seed {
    point p;
    int hardness;
} seed;

int generate_dungeon(dungeon *dungeon,int num_rooms);

#endif