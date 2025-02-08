#ifndef TYPES_H
#define TYPES_H

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
} dungeon;

#endif