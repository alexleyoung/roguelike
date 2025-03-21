#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21

#define IN_BOUNDS(r,c) (r < 1 || r > DUNGEON_HEIGHT - 2 || c < 1 || c > DUNGEON_WIDTH - 2) ? 0 : 1

typedef struct point {
    uint8_t r, c;
} point;

#endif
