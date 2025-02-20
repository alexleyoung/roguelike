#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"

#define PLAYER_TRAIT -1

typedef struct character {
    int id;
    point pos;
    int traits;
    int speed;
} character;

// all available moves characters can make identified by type?
// unless moves are literally just character movement *ASK JERMY*
// 0: move
// 1: interact
typedef struct move {
    int type;
} move;

#endif
