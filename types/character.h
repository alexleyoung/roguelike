#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"

#define PLAYER_TRAIT 33

typedef struct character {
    int id;
    point pos;
    int traits;
    int speed;
    char sprite;
    int (*dist_to_player)[DUNGEON_HEIGHT][DUNGEON_WIDTH];
} character;

/*typedef struct character {*/
/*    int id;*/
/*    point pos;*/
/*    int traits;*/
/*    int speed;*/
/*    char sprite;*/
/*    int (*dist_to_player)[DUNGEON_HEIGHT][DUNGEON_WIDTH];*/
/*} character;*/
#endif
