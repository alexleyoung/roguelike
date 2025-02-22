#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"

#define PLAYER_TRAIT -1

typedef struct character {
    int id;
    point pos;
    int traits;
    int speed;
    char sprite;
} character;

#endif
