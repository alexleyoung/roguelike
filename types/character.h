#ifndef CHARACTER_H
#define CHARACTER_H

#include "dungeon.h"

#define PLAYER_TRAIT -1

typedef struct character {
    int traits;
    int speed;
    point pos;
    point target;
} character;

#endif
