#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21

typedef struct point {
    uint8_t r, c;
} point;

#endif
