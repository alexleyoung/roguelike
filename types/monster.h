#ifndef MONSTER_H
#define MONSTER_H

#include "dungeon.h"

typedef struct character {
    int traits;
    int speed;
    point pos;
    point target;
} character;

#endif
